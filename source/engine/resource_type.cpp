
#include "resource_type.h"
#include "loco.h"

namespace loco
{
	Mesh Mesh::invalid = { { { Renderer::VertexBufferHandle::invalid, Renderer::IndexBufferHandle::invalid } } };
	bool Mesh::operator == (Mesh const& in) const
	{
		return submeshes == in.submeshes;
	}

	bool SubMesh::operator == (SubMesh const& in) const
	{
		return (vertex_buffer == in.vertex_buffer) && (index_buffer == in.index_buffer);
	}
	
	MaterialPtr MaterialPtr::invalid = MaterialPtr(nullptr);

	MaterialPtr MaterialPtr::duplicate() const
	{
		return MaterialPtr(new Material(*(this->get())));
	}

	unsigned UniformType_size[Renderer::UniformType::Enum::Count] =
	{
		1, //Float,
		2, //Vector2,
		3, //Vector3,
		4, //Vector4,
		9, //Matrix3x3,
		16,//Matrix4x4,
		0, //Texture,
	};

	MaterialPtr::MaterialPtr(Material* mat)
		: std::shared_ptr<Material>(mat)
	{

	}

	void Material::set_shader(Renderer::ShaderHandle vertex_shader, Renderer::ShaderHandle pixel_shader)
	{
		_program = renderer.create_program(vertex_shader, pixel_shader);
	}

	void Material::set(const char* name, Renderer::UniformType::Enum type, const float* data, unsigned size)
	{
		UniformInfo& info = create_uniform_param(name, type, size);
		LOCO_ASSERTF(size == info.array_size, "Material parameter \"%s\" size doesn't match with the previous definition (previous size:%d, next size:%d)", name, info.array_size, size);
		
		memcpy((void*)(_uniform_buffer.data() + info.buffer_offset), data, sizeof(float)* size * UniformType_size[info.type]);
	}

	void Material::set(const char* name, Renderer::TextureHandle texture, uint32_t flags)
	{
		TextureInfo& info = create_texture_param(name);
		info.texture = texture;
		info.flags = flags;
	}

	Material::UniformInfo&  Material::create_uniform_param(const char* name, Renderer::UniformType::Enum type, unsigned array_size)
	{
		unsigned uniform_index = 0;
		HashedString hashed_name = hash_string(name);
		auto it = _uniform_map.find(hashed_name);
		if (it == _uniform_map.end())
		{
			UniformInfo info;
			info.type = type;
			info.array_size = array_size;
			info.buffer_offset = _uniform_buffer.size();
			info.uniform = renderer.create_uniform(name, type, array_size);

			uniform_index = _uniform_infos.size();
			_uniform_map[hashed_name] = _uniform_infos.size();
			_uniform_infos.push_back(info);
			_uniform_buffer.insert(_uniform_buffer.end(), UniformType_size[type] * array_size, 0.0f);
		}
		else
		{
			Renderer::UniformType::Enum old_type = _uniform_infos[it->second].type;
			LOCO_ASSERTF(type == old_type, "Material parameter \"%s\" type doesn't match with the previous definition \n(previous type:%d, new type:%d)", name, old_type, type);
			uniform_index = it->second;
		}

		return _uniform_infos[uniform_index];
	}

	Material::TextureInfo& Material::create_texture_param(const char* name)
	{
		HashedString hashed_name = hash_string(name);
		auto it = _texture_map.find(hashed_name);
		if (it == _texture_map.end())
		{
			TextureInfo info;
			info.uniform = renderer.create_uniform(name, Renderer::UniformType::Texture, 1);
			info.texture = { 0 };
			info.flags = 0;
			
			_texture_map[hashed_name] = _texture_infos.size();
			_texture_infos.push_back(info);

			it = _texture_map.end();
			it--;
		}

		return _texture_infos[it->second];
	}

} // loco
