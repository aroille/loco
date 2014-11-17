#ifndef RENDERER_H_HEADER_GUARD
#define RENDERER_H_HEADER_GUARD

namespace loco
{
	struct Renderer
	{
		static void init();
		static void reset(unsigned width, unsigned height);
		static void shutdown();
	};
}

#endif // RENDERER_H_HEADER_GUARD