#ifndef RENDERER_H_HEADER_GUARD
#define RENDERER_H_HEADER_GUARD

#include <stdint.h> // uint16_t
#include <vector>
#include <map>

///
#define LOCO_RENDERER_HANDLE(_name) \
struct _name { \
	uint16_t idx; \
	static _name invalid; \
	bool operator==(_name const& in) const; \
};

namespace loco
{
	struct Memory;
	class MaterialData;
	struct Mesh;

	/// LOCO Renderer 
	///
	/// In charge of:
	///		- Create / Update / Destroy GPU Resource
	///		- Submit draw call from resource manager resources (Mesh, Material)
	/// 
	/// @remarks
	///		Memory* provide as parameters to every Renderer methods, such as 
	///		create_texture(Memory*), must be valid for at least 2 frames
	///
	/// @remarks
	///		Renderer is the only class talking to the backend abstraction library BGFX
	///
	class Renderer
	{
	public:
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

		struct Attrib
		{
			/// Corresponds to vertex shader attribute
			enum Enum
			{
				Position,  //!< a_position
				Normal,    //!< a_normal
				Tangent,   //!< a_tangent
				Bitangent, //!< a_bitangent
				Color0,    //!< a_color0
				Color1,    //!< a_color1
				Indices,   //!< a_indices
				Weight,    //!< a_weight
				TexCoord0, //!< a_texcoord0
				TexCoord1, //!< a_texcoord1
				TexCoord2, //!< a_texcoord2
				TexCoord3, //!< a_texcoord3
				TexCoord4, //!< a_texcoord4
				TexCoord5, //!< a_texcoord5
				TexCoord6, //!< a_texcoord6
				TexCoord7, //!< a_texcoord7

				Count
			};
		};

		struct AttribType
		{
			/// Vertex attribute type
			enum Enum
			{
				Uint8,
				Int16,
				Half,
				Float,

				Count
			};
		};

		/// Attribute declaration
		struct VertexAttribDecl
		{
			Attrib::Enum attrib;	///< Attribute semantics
			uint8_t num;			///< 1, 2, 3 or 4
			AttribType::Enum type;	///< Attribute type
			bool normalized;		///< When using fixed point AttribType (f.e. Uint8)
									///   value will be normalized for vertex shader usage. When normalized
									///   is set to true, AttribType::Uint8 value in range 0-255 will be
									///   in range 0.0-1.0 in vertex shader.
			bool asInt;				///< Packaging rule for vertexPack, vertexUnpack, and
									///   vertexConvert for AttribType::Uint8 and AttribType::Int16.
									///   Unpacking code must be implemented inside vertex shader.
		};

		/// Vertex declaration
		typedef std::vector<VertexAttribDecl> VertexDecl;

		struct UniformType
		{
			/// Shader uniform type enumeration
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

		// GPU resource handles :
		LOCO_RENDERER_HANDLE(TextureHandle);
		LOCO_RENDERER_HANDLE(VertexDeclHandle);
		LOCO_RENDERER_HANDLE(VertexBufferHandle);
		LOCO_RENDERER_HANDLE(IndexBufferHandle);
		LOCO_RENDERER_HANDLE(ShaderHandle);
		LOCO_RENDERER_HANDLE(ProgramHandle);
		LOCO_RENDERER_HANDLE(UniformHandle);

		/// Initialize renderer
		void init();

		/// Shutdown renderer
		void shutdown();

		/// Return the type of the current backend
		Type::Enum type();

		/// Return the name of the backend
		const char* type_name(Type::Enum type);

		/// Set Main window size
		void reset(unsigned width, unsigned height);

		/// Return shader_extention name (according to the current backend)
		inline const char* shader_extention() { return _shader_extention; }

		/// Create texture from memory buffer
		TextureHandle create_texture(const Memory* memory);

		/// Destroy texture
		void destroy_texture(TextureHandle handle);

		/// Create program with vertex and pixel shaders
		ProgramHandle create_program(ShaderHandle vertex_shader, ShaderHandle pixel_shader);
		
		/// Destroy program
		void destroy_program(ProgramHandle handle);

		/// Create shader from memory buffer.
		ShaderHandle create_shader(const Memory* memory);

		/// Destroy shader
		void destroy_shader(ShaderHandle handle);

		/// Create shader uniform 
		UniformHandle create_uniform(const char* name, UniformType::Enum type, unsigned array_size);
		
		/// Destroy shader uniform
		void destroy_uniform(UniformHandle handle);

		/// Create static vertex buffer from memory buffer
		VertexBufferHandle create_vertex_buffer(const Memory* memory, const VertexDecl& decl);

		/// Destroy static vertex buffer.
		void destroy_vertex_buffer(VertexBufferHandle handle);

		/// Create static index buffer from memory buffer
		IndexBufferHandle create_index_buffer(const Memory* memory);

		/// Destroy static index buffer.
		void destroy_index_buffer(IndexBufferHandle handle);

		/// Submit draw call
		///
		/// @param view_id View target
		/// @param Mesh mesh to draw
		/// @param material Material used on every submeshes of mesh
		/// @param model_matrix World transform matrix of the mesh
		///
		void submit(uint8_t view_id, const Mesh& mesh, const MaterialData* material, const void* model_matrix);

	private:
		/// Bind a material
		void bind_material(const MaterialData* material);

	private:
		char _shader_extention[16];						///< shader extention name (according to the current backend)
		std::map<uint32_t, ProgramHandle> _programs;	///< key create from vertex and pixel shader handles
	};
}

#endif // RENDERER_H_HEADER_GUARD