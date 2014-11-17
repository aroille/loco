#include "renderer.h"
#include "bgfx.h"
#include "Memory.h"

namespace loco
{
	void Renderer::init()
	{
		bgfx::init();
	}

	void Renderer::reset(unsigned width, unsigned height)
	{
		uint32_t reset = BGFX_RESET_VSYNC;

		bgfx::reset(width, height, reset);
	}

	void Renderer::shutdown()
	{
		bgfx::shutdown();
	}

	Renderer::TextureHandle Renderer::create_texture(Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		bgfx::TextureHandle bgfx_handle = bgfx::createTexture(bgfx_mem);

		return Renderer::TextureHandle{ bgfx_handle.idx };
	}

	void Renderer::destroy_texture(TextureHandle handle)
	{
		bgfx::destroyTexture(bgfx::TextureHandle{ handle.idx });
	}
}