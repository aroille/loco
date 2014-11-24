#include "renderer.h"
#include "bgfx.h"
#include "memory_utils.h"

#define LOCO_TO_BGFX(handle) { handle.idx }
#define BGFX_TO_LOCO(handle) { handle.idx }

namespace loco
{
namespace renderer
{
	bgfx::UniformType::Enum UniformType_convert[UniformType::Count] =
	{
		bgfx::UniformType::Uniform1f,
		bgfx::UniformType::End,
		bgfx::UniformType::Uniform1fv,
		bgfx::UniformType::Uniform2fv,
		bgfx::UniformType::Uniform3fv,
		bgfx::UniformType::Uniform4fv,
		bgfx::UniformType::Uniform3x3fv,
		bgfx::UniformType::Uniform4x4fv
	};


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
	TextureHandle create_texture(const Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		bgfx::TextureHandle bgfx_handle = bgfx::createTexture(bgfx_mem);

		return BGFX_TO_LOCO(bgfx_handle);
	}

	//==========================================================================
	void destroy_texture(TextureHandle handle)
	{
		bgfx::destroyTexture(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	UniformHandle create_uniform(const char* name, UniformType::Enum type, unsigned array_size)
	{
		bgfx::UniformHandle bgfx_handle = bgfx::createUniform(name, UniformType_convert[type], array_size);
		return BGFX_TO_LOCO(bgfx_handle);
	}

	//==========================================================================
	void set_uniform(UniformHandle handle, const void* value, unsigned array_size)
	{
		bgfx::setUniform(LOCO_TO_BGFX(handle), value, array_size);
	}

	//==========================================================================
	void destroy_uniform(UniformHandle handle)
	{
		bgfx::destroyUniform(LOCO_TO_BGFX(handle));
	}

} // renderer
} // loco
