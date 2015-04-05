#ifndef RENDERERS_H_HEADER_GUARD
#define RENDERERS_H_HEADER_GUARD

#include "loco.h"
#include "math_types.h"
#include "type.h"

namespace loco
{
	using namespace math;

	struct World;
	/*
	struct RenderTarget
	{
		char name[64];
		Vector2 scale;
		Renderer::TextureFormat format;
	};

	RenderTarget global[4] =
	{
		{ "depth_stencil", Vector2::one, Renderer::TextureFormat::D24S8},
		{ "albedo", Vector2::one, Renderer::TextureFormat::RGBA32 },
		{ "normal", Vector2::one, Renderer::TextureFormat::RGBA32 },
		{ "light_accumulation", Vector2::one, Renderer::TextureFormat::RGBA16F }
	};
	*/

	struct DeferredResources
	{
		backend::TextureHandle depth_stencil;
		backend::TextureHandle albedo;
		backend::TextureHandle normal;
		backend::TextureHandle light_accumulation;

		uint32 last_width;
		uint32 last_height;

		void update(uint32 width, uint32 height);
	};

	extern void renderer_deferred(const World& world, Entity camera, Viewport viewport, DeferredResources& deferred_resources);

} // loco

#endif // RENDERERS_H_HEADER_GUARD