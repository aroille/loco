#ifndef RENDERER_H_HEADER_GUARD
#define RENDERER_H_HEADER_GUARD

#include <stdint.h> // uint16_t

namespace loco
{
	struct Memory;

	namespace renderer
	{
		struct TextureHandle		{ uint16_t idx; };
		struct VertexDeclHandle		{ uint16_t idx; };
		struct VertexBufferHandle	{ uint16_t idx; };
		struct IndexBufferHandle	{ uint16_t idx; };
		struct ShaderHandle			{ uint16_t idx; };
		struct ProgramHandle		{ uint16_t idx; };
		struct UniformHandle		{ uint16_t idx; };

		void init();
		void reset(unsigned width, unsigned height);
		void shutdown();

		TextureHandle create_texture(Memory* memory);
		void destroy_texture(TextureHandle texture);
	};
}

#endif // RENDERER_H_HEADER_GUARD