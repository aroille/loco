#include "renderer.h"
#include "bgfx.h"

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
}