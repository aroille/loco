#ifndef RESOURCE_TYPE_H_HEADER_GUARD
#define RESOURCE_TYPE_H_HEADER_GUARD

#include "murmur_hash.h"
#include "renderer.h"

#include <map>
#include <memory>
#include <vector>

namespace loco
{
	/// Texture ==========================================

	typedef renderer::TextureHandle Texture;

	/// Shader ===========================================

	typedef renderer::ShaderHandle Shader;

	/// Mesh =============================================

	struct SubMesh
	{
		renderer::VertexBufferHandle vertex_buffer;
		renderer::IndexBufferHandle index_buffer;
	};

	struct Mesh
	{
		std::vector<SubMesh> submeshes;

		static Mesh invalid;
	};

	/// Material =========================================

	typedef std::shared_ptr<Material> MaterialPtr;
	//typedef Material MaterialPtr;

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

		std::map<HashedString, unsigned> _uniform_map;
		std::map<HashedString, unsigned> _texture_map;

		renderer::ProgramHandle _program;
		std::vector<UniformInfo> _uniform_infos;
		std::vector<TextureInfo> _texture_infos;
		std::vector<float> _uniform_buffer;

	private:

		UniformInfo&  create_uniform_param(char* name, renderer::UniformType::Enum type, unsigned array_size);
		TextureInfo&  create_texture_param(char* name);
	};

	
}

#endif // RESOURCE_TYPE_H_HEADER_GUARD