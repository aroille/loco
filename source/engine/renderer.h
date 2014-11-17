#ifndef RENDERER_H_HEADER_GUARD
#define RENDERER_H_HEADER_GUARD

namespace loco
{
	class Memory;

	struct Renderer
	{
		struct TextureHandle
		{
			unsigned short idx;
		};

		static void init();
		static void reset(unsigned width, unsigned height);
		static void shutdown();

		static TextureHandle create_texture(Memory* memory);
		static void destroy_texture(TextureHandle texture);
	};
}

#endif // RENDERER_H_HEADER_GUARD