#ifndef RENDERER_H_HEADER_GUARD
#define RENDERER_H_HEADER_GUARD

#include <stdint.h> // uint16_t

namespace loco
{
	struct Memory;

	namespace renderer
	{
		struct UniformType
		{
			enum Enum
			{
				Uniform1f,
				End,

				Uniform1fv,
				Uniform2fv,
				Uniform3fv,
				Uniform4fv,
				Uniform3x3fv,
				Uniform4x4fv,

				Count
			};
		};

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

		TextureHandle create_texture(const Memory* memory);
		void destroy_texture(TextureHandle handle);

		UniformHandle create_uniform(const char* name, UniformType::Enum type, unsigned array_size);
		void set_uniform(UniformHandle handle, const void* value, unsigned array_size);
		void destroy_uniform(UniformHandle handle);

	};
}

#endif // RENDERER_H_HEADER_GUARD