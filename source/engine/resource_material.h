#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

#include "resource_manager.h"

#include "debug.h"
#include "murmur_hash.h"

#include <map>
#include <vector>

namespace loco
{


	unsigned UniformType_size[renderer::UniformType::Enum::Count] =
	{
		1, // Uniform1f,
		0, // End,

		1, // Uniform1fv,
		2, // Uniform2fv,
		3,// Uniform3fv,
		4,// Uniform4fv,
		9,// Uniform3x3fv,
		16,// Uniform4x4fv,
	};

	struct UniformInfo
	{
		char							name[LOCO_UNIFORM_LENGTH];
		renderer::UniformType::Enum		type;
		unsigned						array_size;
		unsigned						buffer_offset;
		renderer::UniformHandle			handle;
	};

	class Material
	{
	public:
		void create_uniform(const char* name, renderer::UniformType::Enum type, unsigned array_size = 1)
		{
			UniformInfo info;
			strcpy(info.name, name);
			info.type = type;
			info.array_size = array_size;
			info.buffer_offset = _uniform_buffer.size();
			info.handle = renderer::create_uniform(name, type, array_size);

			_uniform_infos[hash_string(name)] = info;
			_uniform_buffer.insert(_uniform_buffer.end(), UniformType_size[type], 0.0f);
		}

		void set_uniform(char* name, float* data, unsigned size = 0)
		{
			auto it = _uniform_infos.find(hash_string(name));
			LOCO_ASSERTF(it != _uniform_infos.end(), "Shader uniform unknown : %s", name);
			
			UniformInfo& info = (*it).second;
			unsigned size_to_copy = size > 0 ? size : info.array_size;
			memcpy((void*)(_uniform_buffer.data() + info.buffer_offset), data, size_to_copy * UniformType_size[info.type]);
			renderer::set_uniform(info.handle, (void*)data, size_to_copy);
		}
	private:
		std::map<HashedString, UniformInfo> _uniform_infos;
		std::vector<float> _uniform_buffer;
	};

	/*
	template<> Texture ResourceManager::create(const Memory* mem) const
	{
		// Create hardware resource
		renderer::TextureHandle handle = renderer::create_texture(mem);

		// Create a new ResourceManager::Texture instance
		Texture texture { handle };

		return texture;
	}

	template<> void ResourceManager::destroy(const Texture& texture) const
	{
		renderer::destroy_texture(texture.handle);
	}
	*/
}

#endif // RESOURCE_MATERIAL_H_HEADER_GUARD