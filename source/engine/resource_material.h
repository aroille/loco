#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

#include "renderer.h"

#include "murmur_hash.h"
#include <map>
#include <vector>

namespace loco
{
	class Material
	{
	public:

		struct UniformInfo
		{
			renderer::UniformHandle			uniform;
			uint16_t						buffer_offset;
			uint16_t						array_size;
			renderer::UniformType::Enum		type;
		};

		struct TextureInfo
		{
			renderer::UniformHandle			uniform;
			renderer::TextureHandle			texture;
			uint32_t						flags;
		};

		void set_shader(renderer::ShaderHandle vertex_shader, renderer::ShaderHandle pixel_shader);

		void set(char* name, renderer::UniformType::Enum type, float* data, unsigned size = 1);
		void set(char* name, renderer::TextureHandle texture, uint32_t flags = 0);

		renderer::ProgramHandle _program;
		std::map<HashedString, unsigned> _uniform_map;
		std::map<HashedString, unsigned> _texture_map;
		std::vector<Material::UniformInfo> _uniform_infos;
		std::vector<TextureInfo> _texture_infos;
		std::vector<float> _uniform_buffer;

	private:

		UniformInfo&  create_uniform_param(char* name, renderer::UniformType::Enum type, unsigned array_size);
		TextureInfo&  create_texture_param(char* name);
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