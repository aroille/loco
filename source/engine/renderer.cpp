#include "renderer.h"
#include "bgfx.h"
#include "memory.h"

#define CONVERT_HANDLE(handle) { handle.idx }

namespace loco
{
namespace renderer
{
	//==========================================================================
	void init()
	{
		bgfx::init();
	}

	//==========================================================================
	void reset(unsigned width, unsigned height)
	{
		uint32_t reset = BGFX_RESET_VSYNC;

		bgfx::reset(width, height, reset);
	}

	//==========================================================================
	void shutdown()
	{
		bgfx::shutdown();
	}

	//==========================================================================
	TextureHandle create_texture(Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		bgfx::TextureHandle bgfx_handle = bgfx::createTexture(bgfx_mem);

		return CONVERT_HANDLE(bgfx_handle);
	}

	//==========================================================================
	void destroy_texture(TextureHandle handle)
	{
		bgfx::destroyTexture(CONVERT_HANDLE(handle));
	}

} // renderer
} // loco
