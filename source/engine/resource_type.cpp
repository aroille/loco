
#include "resource_type.h"
#include "loco.h"

#include <algorithm>


namespace loco{
namespace resource{

	Mesh Mesh::invalid = Mesh(nullptr);

#ifdef LOCO_USE_HOT_RELOAD
	std::list<MaterialData*> MaterialData::_all_materials = std::list<MaterialData*>();
#endif // LOCO_USE_HOT_RELOAD

	Mesh::Mesh(MeshData* mat)
		: std::shared_ptr<MeshData>(mat)
	{

	}

	Mesh Mesh::duplicate() const
	{
		return Mesh(new MeshData(*(this->get())));
	}

	Material Material::invalid = Material(nullptr);

	Material::Material(MaterialData* mat)
		: std::shared_ptr<MaterialData>(mat)
	{

	}

	Material Material::duplicate() const
	{
		return Material(new MaterialData(*(this->get())));
	}


	unsigned UniformType_size[(uint32)backend::UniformType::Count] =
	{
		1, //Float,
		2, //Vector2,
		3, //Vector3,
		4, //Vector4,
		9, //Matrix3x3,
		16,//Matrix4x4,
		0, //Texture,
	};


	MaterialData::MaterialData()
	{
#ifdef LOCO_USE_HOT_RELOAD
		_all_materials.push_back(this);
#endif // LOCO_USE_HOT_RELOAD
	}

	MaterialData::~MaterialData()
	{
#ifdef LOCO_USE_HOT_RELOAD
		if (!_all_materials.empty())
			_all_materials.remove(this);
#endif // LOCO_USE_HOT_RELOAD
	}

	void MaterialData::set_shader(backend::ShaderHandle vertex_shader, backend::ShaderHandle pixel_shader)
	{
		_program = backend::create_program(vertex_shader, pixel_shader);
	}

	void MaterialData::set(const char* name, backend::UniformType type, const float* data, unsigned size)
	{
		UniformInfo& info = create_uniform_param(name, type, size);
		LOCO_ASSERTF(size == info.array_size, "Material parameter \"%s\" size doesn't match with the previous definition (previous size:%d, next size:%d)", name, info.array_size, size);

		memcpy((void*)(_uniform_buffer.data() + info.buffer_offset), data, sizeof(float)* size * UniformType_size[(uint32)info.type]);
	}

	void MaterialData::set(const char* name, backend::TextureHandle texture, uint32 flags)
	{
		TextureInfo& info = create_texture_param(name);
		info.texture = texture;
		info.flags = flags;
	}

	MaterialData::UniformInfo&  MaterialData::create_uniform_param(const char* name, backend::UniformType type, unsigned array_size)
	{
		unsigned uniform_index = 0;
		HashedString hashed_name = hash_string(name);
		auto it = _uniform_map.find(hashed_name);
		if (it == _uniform_map.end())
		{
			UniformInfo info;
			info.type = type;
			info.array_size = array_size;
			info.buffer_offset = (uint16)_uniform_buffer.size();
			info.uniform = backend::create_uniform(name, type, array_size);

			uniform_index = (unsigned)_uniform_infos.size();
			_uniform_map[hashed_name] = (unsigned)_uniform_infos.size();
			_uniform_infos.push_back(info);
			_uniform_buffer.insert(_uniform_buffer.end(), UniformType_size[(uint32)type] * array_size, 0.0f);
		}
		else
		{
			backend::UniformType old_type = _uniform_infos[it->second].type;
			LOCO_ASSERTF(type == old_type, "Material parameter \"%s\" type doesn't match with the previous definition \n(previous type:%d, new type:%d)", name, old_type, type);
			uniform_index = it->second;
		}

		return _uniform_infos[uniform_index];
	}

	MaterialData::TextureInfo& MaterialData::create_texture_param(const char* name)
	{
		HashedString hashed_name = hash_string(name);
		auto it = _texture_map.find(hashed_name);
		if (it == _texture_map.end())
		{
			TextureInfo info;
			info.uniform = backend::create_uniform(name, backend::UniformType::Texture, 1);
			info.texture = { 0 };
			info.flags = 0;

			_texture_map[hashed_name] = (unsigned)_texture_infos.size();
			_texture_infos.push_back(info);

			it = _texture_map.end();
			it--;
		}

		return _texture_infos[it->second];
	}

} // resource
} // loco
