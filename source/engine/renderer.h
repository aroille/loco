#ifndef RENDERER_H_HEADER_GUARD
#define RENDERER_H_HEADER_GUARD

#include <stdint.h> // uint16_t

#define LOCO_RENDERER_HANDLE(_name) \
struct _name { uint16_t idx; };

namespace loco
{
	struct Memory;

	namespace renderer
	{
		LOCO_RENDERER_HANDLE(TextureHandle);
		LOCO_RENDERER_HANDLE(VertexDeclHandle);
		LOCO_RENDERER_HANDLE(VertexBufferHandle);
		LOCO_RENDERER_HANDLE(IndexBufferHandle);
		LOCO_RENDERER_HANDLE(ShaderHandle);
		LOCO_RENDERER_HANDLE(ProgramHandle);
		LOCO_RENDERER_HANDLE(UniformHandle);

		void init();
		void reset(unsigned width, unsigned height);
		void shutdown();

		TextureHandle create_texture(Memory* memory);
		void destroy_texture(TextureHandle texture);
	};
}

#endif // RENDERER_H_HEADER_GUARD