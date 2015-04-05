#ifndef BACKEND_H_HEADER_GUARD
#define BACKEND_H_HEADER_GUARD

#include "math_types.h"
#include "type.h"
#include <vector>

#define LOCO_TEXTURE_U_MIRROR						UINT32_C(0x00000001)
#define LOCO_TEXTURE_U_CLAMP            UINT32_C(0x00000002)
#define LOCO_TEXTURE_V_MIRROR           UINT32_C(0x00000004)
#define LOCO_TEXTURE_V_CLAMP            UINT32_C(0x00000008)
#define LOCO_TEXTURE_W_MIRROR           UINT32_C(0x00000010)
#define LOCO_TEXTURE_W_CLAMP            UINT32_C(0x00000020)
#define LOCO_TEXTURE_MIN_POINT          UINT32_C(0x00000040)
#define LOCO_TEXTURE_MIN_ANISOTROPIC    UINT32_C(0x00000080)
#define LOCO_TEXTURE_MAG_POINT          UINT32_C(0x00000100)
#define LOCO_TEXTURE_MAG_ANISOTROPIC    UINT32_C(0x00000200)
#define LOCO_TEXTURE_MIP_POINT          UINT32_C(0x00000400)

namespace loco
{
	struct Viewport
	{
		uint16 x;
		uint16 y;
		uint16 width;
		uint16 height;
	};


	struct Memory;

	namespace resource
	{
		struct DefaultResources;
		class MaterialData;
		class Mesh;
	}

	using namespace resource;
	using namespace math;

	namespace backend
	{
#define LOCO_BACKEND_HANDLE(handle_name) \
		struct handle_name { \
			uint16 idx; \
			static handle_name invalid; \
			bool operator==(handle_name const& in) const; \
			};

		/// Renderer types
		enum class Type
		{
			Null,         //!< No rendering.
			Direct3D9,    //!< Direct3D 9.0
			Direct3D11,   //!< Direct3D 11.0
			OpenGLES = 4, //!< OpenGL ES 2.0+
			OpenGL,       //!< OpenGL 2.1+

			Count, 
		};

		/// Vertex attributes
		enum class VertexAttrib
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

		/// Vertex attribute types
		enum class VertexAttribType
		{
			Uint8,
			Int16,
			Half,
			Float,

			Count
		};

		/// Vertex Attributes declaration
		struct VertexAttribDecl
		{
			VertexAttrib attrib;		///< Attribute semantics
			uint8 num;							///< 1, 2, 3 or 4
			VertexAttribType type;	///< Attribute type
			bool normalized;				///< When using fixed point AttribType (f.e. Uint8)
															///   value will be normalized for vertex shader usage. When normalized
															///   is set to true, AttribType::Uint8 value in range 0-255 will be
															///   in range 0.0-1.0 in vertex shader.
			bool asInt;							///< Packaging rule for vertexPack, vertexUnpack, and
															///   vertexConvert for AttribType::Uint8 and AttribType::Int16.
															///   Unpacking code must be implemented inside vertex shader.
		};

		/// Vertex declaration
		typedef std::vector<VertexAttribDecl> VertexDecl;

		/// Shader uniform type enumeration
		enum class UniformType
		{
			Float,
			Vector2,
			Vector3,
			Vector4,
			Matrix3x3,
			Matrix4x4,
			Texture,

			Count,
		};

		enum class TextureFormat
		{
			BC1,    // DXT1
			BC2,    // DXT3
			BC3,    // DXT5
			BC4,    // LATC1/ATI1
			BC5,    // LATC2/ATI2
			BC6H,   // BC6H
			BC7,    // BC7
			ETC1,   // ETC1 RGB8
			ETC2,   // ETC2 RGB8
			ETC2A,  // ETC2 RGBA8
			ETC2A1, // ETC2 RGB8A1
			PTC12,  // PVRTC1 RGB 2BPP
			PTC14,  // PVRTC1 RGB 4BPP
			PTC12A, // PVRTC1 RGBA 2BPP
			PTC14A, // PVRTC1 RGBA 4BPP
			PTC22,  // PVRTC2 RGBA 2BPP
			PTC24,  // PVRTC2 RGBA 4BPP

			Unknown, // compressed formats above

			R1,
			R8,
			R16,
			R16F,
			R32,
			R32F,
			RG8,
			RG16,
			RG16F,
			RG32,
			RG32F,
			BGRA8,
			RGBA16,
			RGBA16F,
			RGBA32,
			RGBA32F,
			R5G6B5,
			RGBA4,
			RGB5A1,
			RGB10A2,
			R11G11B10F,

			UnknownDepth, // depth formats below

			D16,
			D24,
			D24S8,
			D32,
			D16F,
			D24F,
			D32F,
			D0S8,

			Count
		};

		// GPU resource handles :
		LOCO_BACKEND_HANDLE(TextureHandle);
		LOCO_BACKEND_HANDLE(VertexDeclHandle);
		LOCO_BACKEND_HANDLE(VertexBufferHandle);
		LOCO_BACKEND_HANDLE(IndexBufferHandle);
		LOCO_BACKEND_HANDLE(ShaderHandle);
		LOCO_BACKEND_HANDLE(ProgramHandle);
		LOCO_BACKEND_HANDLE(UniformHandle);

		/// Initialize renderer
		extern void init(Type renderer_type);

		/// Shutdown renderer
		extern void shutdown();

		/// Return the type of the current backend
		extern Type type();

		/// Return the name of the backend
		extern const char* type_name(Type type);

		/// Set Main window size
		extern void reset(unsigned width, unsigned height);

		/// Switch render buffer
		extern void frame();

		/// Return shader_extention name (according to the current backend)
		extern const char* shader_extention();

		/// Create texture from memory buffer
		extern TextureHandle create_texture(const Memory* memory);

		/// Destroy texture
		extern void destroy_texture(TextureHandle handle);

		/// Create program with vertex and pixel shaders
		extern ProgramHandle create_program(ShaderHandle vertex_shader, ShaderHandle pixel_shader);
		
		/// Destroy program
		extern void destroy_program(ProgramHandle handle);

		/// Create shader from memory buffer.
		extern ShaderHandle create_shader(const Memory* memory);

		/// Destroy shader
		extern void destroy_shader(ShaderHandle handle);

		/// Create shader uniform 
		extern UniformHandle create_uniform(const char* name, UniformType type, unsigned array_size);
		
		/// Destroy shader uniform
		extern void destroy_uniform(UniformHandle handle);

		/// Create static vertex buffer from memory buffer
		extern VertexBufferHandle create_vertex_buffer(const Memory* memory, const VertexDecl& decl);

		/// Destroy static vertex buffer.
		extern void destroy_vertex_buffer(VertexBufferHandle handle);

		/// Create static index buffer from memory buffer
		extern IndexBufferHandle create_index_buffer(const Memory* memory);

		/// Destroy static index buffer.
		extern void destroy_index_buffer(IndexBufferHandle handle);

		/// Set viewport 
		extern void set_view_rect(uint8 view_id, Viewport viewport);

		/// Set view and projection matrix
		extern void set_view_transform(uint8 view_id, const Matrix4x4& view_mtx, const Matrix4x4& proj_mtx);

	} // backend
} // loco

#endif // BACKEND_H_HEADER_GUARD