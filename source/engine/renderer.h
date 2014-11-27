#ifndef RENDERER_H_HEADER_GUARD
#define RENDERER_H_HEADER_GUARD

#include <stdint.h> // uint16_t
#include <vector>

namespace bgfx
{
	struct VertexDecl;
}

namespace loco
{
	struct Memory;
	class Material;
	struct Mesh;

	namespace renderer
	{
		struct VertexAttrib
		{
			enum Enum
			{
				Position, 
				Normal,
				Tangent,
				Bitangent,
				Color0,
				Color1,
				Indices,
				Weight,
				TexCoord0,
				TexCoord1,
				TexCoord2,
				TexCoord3,
				TexCoord4,
				TexCoord5,
				TexCoord6,
				TexCoord7,

				Count
			};
		};

		struct AttribType
		{
			enum Enum
			{
				Uint8,
				Int16,
				Half,
				Float,

				Count
			};
		};

		struct UniformType
		{
			enum Enum
			{
				Float,
				Vector2,
				Vector3,
				Vector4,
				Matrix3x3,
				Matrix4x4,
				Texture,
				Count
			};
		};

		struct VertexAttribDecl
		{
			VertexAttrib::Enum attrib;
			uint8_t num;
			AttribType::Enum type;
			bool normalized;
			bool asInt;
		};

		typedef std::vector<VertexAttribDecl> VertexDecl;

		struct TextureHandle		{ uint16_t idx; static TextureHandle invalid; };
		struct VertexDeclHandle		{ uint16_t idx; };
		struct VertexBufferHandle	{ uint16_t idx; };
		struct IndexBufferHandle	{ uint16_t idx; };
		struct ShaderHandle			{ uint16_t idx; static ShaderHandle invalid; };
		struct ProgramHandle		{ uint16_t idx; static ProgramHandle invalid; };
		struct UniformHandle		{ uint16_t idx; };

		void init();
		void reset(unsigned width, unsigned height);
		void shutdown();

		TextureHandle create_texture(const Memory* memory);
		void destroy_texture(TextureHandle handle);

		ProgramHandle create_program(ShaderHandle vertex_shader, ShaderHandle fragment_shader);
		void destroy_program(ProgramHandle handle);

		ShaderHandle create_shader(const Memory* memory);
		void destroy_shader(ShaderHandle handle);

		UniformHandle create_uniform(const char* name, UniformType::Enum type, unsigned array_size);
		void destroy_uniform(UniformHandle handle);

		VertexBufferHandle create_vertex_buffer(const Memory* memory, const VertexDecl& decl);
		VertexBufferHandle create_vertex_buffer(const Memory* memory, const bgfx::VertexDecl& decl);
		void destroy_vertex_buffer(VertexBufferHandle handle);

		IndexBufferHandle create_index_buffer(const Memory* memory);
		void destroy_index_buffer(IndexBufferHandle handle);

		void bind_material(const Material& mat);
		void submit(uint8_t view_id, const Mesh& mesh, const Material& mat, const void* model_matrix);
	};
}

#endif // RENDERER_H_HEADER_GUARD