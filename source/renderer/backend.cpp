#include "backend.h"
#include "memory_utils.h"
#include "log.h"
#include "resource_type.h"

#include "bgfx.h" 
#include <map>

#define LOCO_RENDERER "Renderer" // log module string

#define LOCO_TO_BGFX(handle) { handle.idx }
#define BGFX_TO_LOCO(handle) { handle.idx }


namespace loco
{
	namespace backend
	{

		static char _shader_extention[16];						///< shader extention name (according to the current backend)
		static std::map<uint32, ProgramHandle> _programs;	///< key create from vertex and pixel shader handles


#define LOCO_RENDERER_HANDLE_DEF(handle_name) \
bool handle_name::operator==(handle_name const& in) const { return idx == in.idx; }; \
handle_name handle_name::invalid = { UINT16_MAX };


		using namespace resource;
		using namespace math;

		LOCO_RENDERER_HANDLE_DEF(TextureHandle);
		LOCO_RENDERER_HANDLE_DEF(VertexDeclHandle);
		LOCO_RENDERER_HANDLE_DEF(VertexBufferHandle);
		LOCO_RENDERER_HANDLE_DEF(IndexBufferHandle);
		LOCO_RENDERER_HANDLE_DEF(ShaderHandle);
		LOCO_RENDERER_HANDLE_DEF(ProgramHandle);
		LOCO_RENDERER_HANDLE_DEF(UniformHandle);

		char shader_extentions[(uint32)Type::Count][16] =
		{
			"shader_none",
			"shader_dx9",
			"shader_dx11",
			"shader_gles",
			"shader_glsl"
		};

		bgfx::UniformType::Enum UniformType_convert[(uint32)UniformType::Count] =
		{
			bgfx::UniformType::Uniform1fv,
			bgfx::UniformType::Uniform2fv,
			bgfx::UniformType::Uniform3fv,
			bgfx::UniformType::Uniform4fv,
			bgfx::UniformType::Uniform3x3fv,
			bgfx::UniformType::Uniform4x4fv,
			bgfx::UniformType::Uniform1iv
		};

		//==========================================================================
		void init(Type renderer_type)
		{
			LOCO_LOG_INFO(LOCO_RENDERER, "Initializing");
			bgfx::init((bgfx::RendererType::Enum)renderer_type);
			LOCO_LOG_INFO(LOCO_RENDERER, "%s", bgfx::getRendererName(bgfx::getRendererType()));
			strcpy(_shader_extention, shader_extentions[(uint32)type()]);
		}

		//==========================================================================
		void reset(unsigned width, unsigned height)
		{
			uint32 reset = BGFX_RESET_VSYNC;

			bgfx::reset(width, height, reset);
		}

		//==========================================================================
		void shutdown()
		{
			bgfx::shutdown();
		}

		//==========================================================================
		Type type()
		{
			return (Type)bgfx::getRendererType();
		}

		//==========================================================================
		const char* type_name(Type type)
		{
			return bgfx::getRendererName((bgfx::RendererType::Enum) type);
		}

		//==========================================================================
		void frame()
		{
			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::submit(0);

			bgfx::frame();
		}

		//==========================================================================
		const char* shader_extention() 
		{ 
			return _shader_extention; 
		}

		//==========================================================================
		TextureHandle create_texture(const Memory* memory)
		{
			const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
			return BGFX_TO_LOCO(bgfx::createTexture(bgfx_mem));
		}

		//==========================================================================
		void destroy_texture(TextureHandle handle)
		{
			bgfx::destroyTexture(LOCO_TO_BGFX(handle));
		}

		//==========================================================================
		ProgramHandle create_program(ShaderHandle vsh, ShaderHandle psh)
		{
			ProgramHandle handle;

			uint32 key = vsh.idx + (psh.idx << 16);
			auto it = _programs.find(key);
			if (it != _programs.end())
			{
				handle = it->second;
			}
			else
			{
				handle = BGFX_TO_LOCO(bgfx::createProgram(LOCO_TO_BGFX(vsh), LOCO_TO_BGFX(psh), false));
				_programs[key] = handle;
			}

			return handle;
		}

		//==========================================================================
		void destroy_program(ProgramHandle handle)
		{
			bgfx::destroyProgram(LOCO_TO_BGFX(handle));
		}

		//==========================================================================
		ShaderHandle create_shader(const Memory* memory)
		{
			const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
			return BGFX_TO_LOCO(bgfx::createShader(bgfx_mem));
		}

		//==========================================================================
		void destroy_shader(ShaderHandle handle)
		{
			bgfx::destroyShader(LOCO_TO_BGFX(handle));
		}

		//==========================================================================
		UniformHandle create_uniform(const char* name, UniformType type, unsigned array_size)
		{
			bgfx::UniformHandle bgfx_handle = bgfx::createUniform(name, UniformType_convert[(uint32)type], array_size);
			return BGFX_TO_LOCO(bgfx_handle);
		}

		//==========================================================================
		void destroy_uniform(UniformHandle handle)
		{
			bgfx::destroyUniform(LOCO_TO_BGFX(handle));
		}

		//==========================================================================
		VertexBufferHandle create_vertex_buffer(const Memory* memory, const VertexDecl& decl)
		{
			bgfx::VertexDecl bgfx_decl;
			bgfx_decl.begin();
			for (unsigned i = 0; i < decl.size(); i++)
			{
				const VertexAttribDecl& d = decl[i];
				bgfx_decl.add((bgfx::Attrib::Enum)d.attrib, d.num, (bgfx::AttribType::Enum)d.type, d.normalized, d.asInt);
			}
			bgfx_decl.end();

			const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
			return BGFX_TO_LOCO(bgfx::createVertexBuffer(bgfx_mem, bgfx_decl));
		}

		//==========================================================================
		void destroy_vertex_buffer(VertexBufferHandle handle)
		{
			bgfx::destroyVertexBuffer(LOCO_TO_BGFX(handle));
		}

		//==========================================================================
		IndexBufferHandle create_index_buffer(const Memory* memory)
		{
			const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
			return BGFX_TO_LOCO(bgfx::createIndexBuffer(bgfx_mem));
		}

		//==========================================================================
		void destroy_index_buffer(IndexBufferHandle handle)
		{
			bgfx::destroyIndexBuffer(LOCO_TO_BGFX(handle));
		}

		//==========================================================================
		void set_view_rect(uint8 view_id, Viewport viewport)
		{
			bgfx::setViewRect(view_id, viewport.x, viewport.y, viewport.width, viewport.height);
		}

		//==========================================================================
		void set_view_transform(uint8 view_id, const Matrix4x4& view, const Matrix4x4& proj)
		{
			bgfx::setViewTransform(view_id, (const void*)view.m, (const void*)proj.m);
		}
	} // backend
} // loco
