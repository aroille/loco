#include "renderer.h"
#include "bgfx.h"
#include "memory.h"

namespace loco
{
	namespace renderer
	{
	/*
	typedef bgfx::TextureHandle RendererTextureHandle;
	typedef bgfx::VertexDeclHandle RendererVertexDeclHandle;
	typedef bgfx::VertexBufferHandle RendererVertexBufferHandle;
	typedef bgfx::IndexBufferHandle RendererIndexBufferHandle;
	typedef bgfx::ShaderHandle RendererShaderHandle;
	typedef bgfx::ProgramHandle RendererProgramHandle;
	typedef bgfx::UniformHandle RendererUniformHandle;
	*/

	template<typename To, typename From>
	To convert(From handle)
	{
		return To{ handle.idx };
	}

	void init()
	{
		bgfx::init();
	}

	void reset(unsigned width, unsigned height)
	{
		uint32_t reset = BGFX_RESET_VSYNC;

		bgfx::reset(width, height, reset);
	}

	void shutdown()
	{
		bgfx::shutdown();
	}

	TextureHandle create_texture(Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		bgfx::TextureHandle bgfx_handle = bgfx::createTexture(bgfx_mem);

		return convert<TextureHandle>(bgfx_handle);
	}

	void destroy_texture(TextureHandle handle)
	{
		bgfx::destroyTexture(convert<bgfx::TextureHandle>(handle));
	}

} // renderer
} // loco
