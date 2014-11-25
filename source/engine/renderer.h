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
				Float,
				Vector2,
				Vector3,
				Vector4,
				Matrix3x3,
				Matrix4x4,
				Texture,
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

		ProgramHandle create_program(ShaderHandle vertex_shader, ShaderHandle fragment_shader);
		void destroy_program(ProgramHandle handle);

		ShaderHandle create_shader(const Memory* memory);
		void destroy_shader(ShaderHandle handle);

		UniformHandle create_uniform(const char* name, UniformType::Enum type, unsigned array_size);
		void destroy_uniform(UniformHandle handle);
	};
}

#endif // RENDERER_H_HEADER_GUARD