#include "mesh_render_system.h"
#include "debug.h"

namespace loco
{
	MeshRenderSystem::MeshRenderSystem()
	{
		_data = ComponentData{};
		allocate(16);
	}

	MeshRenderSystem::~MeshRenderSystem()
	{
		free(_data.buffer);
	}

	MeshRenderSystem::Component MeshRenderSystem::create(Entity e)
	{
		LOCO_ASSERTF(!is_valid(lookup(e)), LOCO_LOG_MESHRENDER_SYSTEM, "An entity can't have several MeshRender components in the same world");

		Component c = _handle_mgr.create();

		if ((c.index() + 1) >= _data.capacity)
			allocate(_data.capacity * 2);

		unsigned pos = _data.size;

		_data.transform[pos] = Matrix4x4::identity;
		_data.mesh[pos] = Mesh::invalid;
		_data.component[pos] = c;

		_data.lut[c.index()] = pos;
		_map[e.id] = c;

		++_data.size;

		return c;
	}

	MeshRenderSystem::Component MeshRenderSystem::lookup(Entity e)
	{
		auto it = _map.find(e.id);
		return (it == _map.end()) ? MeshRenderSystem::Component{ -1 } : it->second;
	}

	bool MeshRenderSystem::is_valid(Component c)
	{
		return _handle_mgr.is_alive(c);
	}

	void MeshRenderSystem::destroy(Entity e)
	{
		Component c = lookup(e);
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_MESHRENDER_SYSTEM, "MeshRender component not found for entity (id:%s)", e.id);
		unsigned pos = data_index(c);

		_map.erase(e.id);
		_handle_mgr.destroy(c);

		// move the instance at [size-1] to the initial index of the destroyed instance
		if (_data.size > 1)
			move_instance(_data.size - 1, pos);

		--_data.size;
	}

	void MeshRenderSystem::set_mesh(Component c, const Mesh& mesh)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_MESHRENDER_SYSTEM, "MeshRender component not valid");
		unsigned i = data_index(c);
		_data.mesh[i] = mesh;
	}

	void MeshRenderSystem::move_instance(unsigned from, unsigned to)
	{
		Component from_component = _data.component[from];

		_data.transform[to] = _data.transform[from];
		_data.mesh[to] = _data.mesh[from];
		_data.component[to] = _data.component[from];

		_data.lut[from_component.index()] = to;
	}

	void MeshRenderSystem::allocate(unsigned sz)
	{
		LOCO_ASSERT(sz > _data.size);

		ComponentData new_data;
		unsigned alignement = 16; // Matrix4x4 need to be aligned on 16 octets (Matrix4x4* are first members of the struct)
		const unsigned bytes = sz * (sizeof(Matrix4x4)+sizeof(Mesh)+sizeof(Component)+sizeof(unsigned))+alignement;
		new_data.buffer = (char*)malloc(bytes);
		new_data.size = _data.size;
		new_data.capacity = sz;

		unsigned offset = alignement - ((unsigned)new_data.buffer & (alignement - 1));

		new_data.transform = (Matrix4x4 *)(new_data.buffer + offset);
		new_data.mesh = (Mesh*)(new_data.transform + sz);
		new_data.component = (Component*)(new_data.mesh + sz);
		new_data.lut = (unsigned*)(new_data.component + sz);
		
		memcpy(new_data.transform, _data.transform, _data.size * sizeof(Matrix4x4));
		memcpy(new_data.mesh, _data.mesh, _data.size * sizeof(Mesh));
		memcpy(new_data.component, _data.component, _data.size * sizeof(Component));
		memcpy(new_data.lut, _data.lut, _data.size * sizeof(unsigned));

		memset(new_data.mesh + _data.size, 0, (sz - _data.size)*sizeof(Mesh));

		free(_data.buffer);
		_data = new_data;
	}

	void MeshRenderSystem::sync_transform(unsigned count, Entity* entity, Matrix4x4* transform)
	{
		for (unsigned i = 0; i < count; i++)
		{
			Entity e = *(entity + i);
			auto it = _map.find(e.id);
			if (it != _map.end())
			{
				unsigned pos = _data.lut[it->second.index()];
				_data.transform[pos] = *(transform + i);
			}	
		}
	}


} // loco