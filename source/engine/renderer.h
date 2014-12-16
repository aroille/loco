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
		struct Type
		{
			/// Renderer type enumeration.
			enum Enum
			{
				Null,         //!< No rendering.
				Direct3D9,    //!< Direct3D 9.0
				Direct3D11,   //!< Direct3D 11.0
				OpenGLES = 4, //!< OpenGL ES 2.0+
				OpenGL,       //!< OpenGL 2.1+

				Count
			};
		};

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

		struct TextureHandle		{ uint16_t idx; static TextureHandle invalid; bool operator==(TextureHandle const& in) const; };
		struct VertexDeclHandle		{ uint16_t idx; static VertexDeclHandle invalid; };
		struct VertexBufferHandle	{ uint16_t idx; static VertexBufferHandle invalid; bool operator==(VertexBufferHandle const& in) const; };
		struct IndexBufferHandle	{ uint16_t idx; static IndexBufferHandle invalid; bool operator==(IndexBufferHandle const& in) const; };
		struct ShaderHandle			{ uint16_t idx; static ShaderHandle invalid; bool operator==(ShaderHandle const& in) const; };
		struct ProgramHandle		{ uint16_t idx; static ProgramHandle invalid; };
		struct UniformHandle		{ uint16_t idx; static UniformHandle invalid; };

		void init();
		void reset(unsigned width, unsigned height);
		void create_default_resources();
		void shutdown();
		Type::Enum type();
		const char* type_name(Type::Enum type);

		void bind_material(const Material* material);
		void submit(uint8_t view_id, const Mesh& mesh, const Material* material, const void* model_matrix);

		extern char shader_extention[16];

		// resources management functions :

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

	};
}

#endif // RENDERER_H_HEADER_GUARD