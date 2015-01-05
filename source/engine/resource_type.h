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

	typedef Renderer::TextureHandle Texture;

	/// Shader ===========================================

	typedef Renderer::ShaderHandle Shader;

	/// Mesh =============================================

	struct SubMesh
	{
		Renderer::VertexBufferHandle vertex_buffer;
		Renderer::IndexBufferHandle index_buffer;

		bool operator==(SubMesh const& in) const;
	};

	struct Mesh
	{
		std::vector<SubMesh> submeshes;

		static Mesh invalid;
		bool operator==(Mesh const& in) const;
	};

	/// Material =========================================

	//typedef std::shared_ptr<Material> MaterialPtr;
	class Material : public std::shared_ptr<MaterialData>
	{
	public:
		Material(MaterialData* mat = nullptr);

		static Material invalid;
		Material duplicate() const;
	};

	class MaterialData
	{
	public:

		struct UniformInfo
		{
			Renderer::UniformHandle			uniform;
			uint16_t						buffer_offset;
			uint16_t						array_size;
			Renderer::UniformType::Enum		type;
		};

		struct TextureInfo
		{
			Renderer::UniformHandle			uniform;
			Renderer::TextureHandle			texture;
			uint32_t						flags;
		};

		void set_shader(Renderer::ShaderHandle vertex_shader, Renderer::ShaderHandle pixel_shader);

		void set(const char* name, Renderer::UniformType::Enum type, const float* data, unsigned size = 1);
		void set(const char* name, Renderer::TextureHandle texture, uint32_t flags = 0);
	
		std::map<HashedString, unsigned> _uniform_map;
		std::map<HashedString, unsigned> _texture_map;

		Renderer::ProgramHandle _program;
		std::vector<UniformInfo> _uniform_infos;
		std::vector<TextureInfo> _texture_infos;
		std::vector<float> _uniform_buffer;

	private:

		UniformInfo&  create_uniform_param(const char* name, Renderer::UniformType::Enum type, unsigned array_size);
		TextureInfo&  create_texture_param(const char* name);
	};

	
}

#endif // RESOURCE_TYPE_H_HEADER_GUARD