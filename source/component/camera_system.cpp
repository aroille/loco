#include "camera_system.h"
#include "debug.h"
#include "math_types.h"
#include "math_utils.h"

#include <bx/fpumath.h>

#define LOCO_CAMERA_SYSTEM	"CameraSystem" // log module string

namespace loco
{
	const CameraComponent CameraComponent::null = { -1 };

	CameraSystem::CameraSystem()
	{
		_data = ComponentData{};
		allocate(16);
	}

	CameraSystem::~CameraSystem()
	{
		free(_data.buffer);
	}

	CameraComponent CameraSystem::create(Entity e)
	{
		LOCO_ASSERTF(!is_valid(lookup(e)), LOCO_CAMERA_SYSTEM, "An entity can't have several Camera components in the same world");

		CameraComponent c = { _handle_mgr.create() };

		// expand data buffer if necessary
		if ((c.index() + 1) >= _data.capacity)
			allocate(_data.capacity * 2);

		unsigned pos = _data.size;

		CameraParameters default_data;
		default_data.proj = ProjectionType::Perspective;
		default_data.vertical_fov = 60.0f;
		default_data.near_distance = 0.3f;
		default_data.far_distance = 1000.0f;

		_data.param[pos] = default_data;
		_data.entity[pos] = e;
		_data.component[pos] = c;

		_data.lut[c.index()] = pos;
		_map[e.id] = c;

		++_data.size;

		return c;
	}

	CameraComponent CameraSystem::lookup(Entity e) const
	{
		auto it = _map.find(e.id);
		return (it == _map.end()) ? CameraComponent::null : it->second;
	}

	bool CameraSystem::is_valid(CameraComponent c) const
	{
		return _handle_mgr.is_alive(c.handle);
	}

	void CameraSystem::destroy(Entity e)
	{
		CameraComponent c = lookup(e);
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not found for entity (id:%s)", e.id);
		unsigned pos = data_index(c);

		_map.erase(e.id);
		_handle_mgr.destroy(c.handle);

		// move the instance at [size-1] to the initial index of the destroyed instance
		if (_data.size > 1)
			move_instance(_data.size - 1, pos);

		--_data.size;
	}

	void CameraSystem::move_instance(unsigned from, unsigned to)
	{
		CameraComponent from_component = _data.component[from];

		_data.param[to] = _data.param[from];
		_data.entity[to] = _data.entity[from];
		_data.component[to] = _data.component[from];

		_data.lut[from_component.index()] = to;
	}

	void CameraSystem::allocate(unsigned sz)
	{
		LOCO_ASSERT(sz > _data.size);

		ComponentData new_data;
		const unsigned bytes = sz * (sizeof(CameraParameters) + sizeof(Entity) + sizeof(CameraComponent) + sizeof(unsigned));
		new_data.buffer = (char*)malloc(bytes);
		new_data.size = _data.size;
		new_data.capacity = sz;

		new_data.param = (CameraParameters *)(new_data.buffer);
		new_data.entity = (Entity*)(new_data.param + sz);
		new_data.component = (CameraComponent*)(new_data.entity + sz);
		new_data.lut = (unsigned*)(new_data.component + sz);

		memcpy(new_data.param, _data.param, _data.size * sizeof(CameraParameters));
		memcpy(new_data.entity, _data.entity, _data.size * sizeof(Entity));
		memcpy(new_data.component, _data.component, _data.size * sizeof(CameraComponent));
		memcpy(new_data.lut, _data.lut, _data.size * sizeof(unsigned));

		free(_data.buffer);
		_data = new_data;
	}

	void CameraSystem::gc(const EntityManager& em)
	{
		unsigned alive_in_row = 0;
		while ((_data.size > 0) && (alive_in_row < 4))
		{
			unsigned random_pos = random_in_range(0, _data.size - 1);
			Entity e = _data.entity[random_pos];
			if (em.is_alive(e))
			{
				alive_in_row++;
				continue;
			}

			alive_in_row = 0;
			destroy(e);
		}
	}

	CameraSystem::ProjectionType::Enum CameraSystem::projection_type(CameraComponent c) const
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		return _data.param[i].proj;
	}

	void CameraSystem::set_projection_type(CameraComponent c, ProjectionType::Enum p)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		_data.param[i].proj = p;
	}

	float CameraSystem::near_distance(CameraComponent c) const
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		return _data.param[i].near_distance;
	}

	void CameraSystem::set_near_distance(CameraComponent c, float d)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		_data.param[i].near_distance = d;
	}

	float CameraSystem::far_distance(CameraComponent c) const
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		return _data.param[i].far_distance;
	}

	void CameraSystem::set_far_distance(CameraComponent c, float d)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		_data.param[i].far_distance = d;
	}

	float CameraSystem::fov(CameraComponent c) const
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		return _data.param[i].vertical_fov;
	}

	void CameraSystem::set_fov(CameraComponent c, float fov)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		_data.param[i].vertical_fov = fov;
	}

	float CameraSystem::ortho_size(CameraComponent c) const
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		return _data.param[i].size;
	}

	void CameraSystem::set_ortho_size(CameraComponent c, float size)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		_data.param[i].size = size;
	}

	Matrix4x4 CameraSystem::projection_matrix(CameraComponent c, float aspect_ratio) const
	{
		LOCO_ASSERTF(is_valid(c), LOCO_CAMERA_SYSTEM, "Camera component not valid");
		unsigned i = data_index(c);
		CameraParameters& p = _data.param[i];

		Matrix4x4 m;
		if (p.proj == ProjectionType::Perspective)
		{
			bx::mtxProj((float*)m.val, p.vertical_fov, aspect_ratio, p.near_distance, p.far_distance);
		}
		else
		{
			bx::mtxOrtho((float*)m.val, -p.size, p.size, -p.size / aspect_ratio, p.size / aspect_ratio, p.near_distance, p.far_distance);
		}
		return m;
	}

} // loco