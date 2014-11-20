
#ifndef BGFX_TEMP_H_HEADER_GUARD
#define BGFX_TEMP_H_HEADER_GUARD

#include "entry.h"
#include <bx/readerwriter.h>
#include <bx/fpumath.h>
#include "bgfx.h"

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};


static PosColorVertex s_cubeVertices[8] =
{
	{ -1.0f, 1.0f, 1.0f, 0xff000000 },
	{ 1.0f, 1.0f, 1.0f, 0xff0000ff },
	{ -1.0f, -1.0f, 1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f, 1.0f, 0xff00ffff },
	{ -1.0f, 1.0f, -1.0f, 0xffff0000 },
	{ 1.0f, 1.0f, -1.0f, 0xffff00ff },
	{ -1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeIndices[36] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

static const bgfx::Memory* loadMem(bx::FileReaderI* _reader, const char* _filePath)
{
	int i = 0;
	if (0 == bx::open(_reader, _filePath))
	{
		uint32_t size = (uint32_t)bx::getSize(_reader);
		const bgfx::Memory* mem = bgfx::alloc(size + 1);
		bx::read(_reader, mem->data, size);
		bx::close(_reader);
		mem->data[mem->size - 1] = '\0';
		return mem;
	}

	return NULL;
}

static bgfx::ShaderHandle loadShader(bx::FileReaderI* _reader, const char* _name)
{
	char filePath[512];

	const char* shaderPath = "../../../samples/resources/shaders/dx9/";

	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Direct3D11:
		shaderPath = "../../../samples/resources/shaders/dx11/";
		break;

	case bgfx::RendererType::OpenGL:
		shaderPath = "../../../samples/resources/shaders/glsl/";
		break;

	case bgfx::RendererType::OpenGLES:
		shaderPath = "../../../samples/resources/shaders/gles/";
		break;

	default:
		break;
	}

	strcpy(filePath, shaderPath);
	strcat(filePath, _name);
	strcat(filePath, ".bin");

	return bgfx::createShader(loadMem(_reader, filePath));
}

bgfx::ShaderHandle loadShader(const char* _name)
{
	return loadShader(loco::entry::get_file_reader(), _name);
}

bgfx::ProgramHandle loadProgram(bx::FileReaderI* _reader, const char* _vsName, const char* _fsName)
{
	bgfx::ShaderHandle vsh = loadShader(_reader, _vsName);
	bgfx::ShaderHandle fsh = loadShader(_reader, _fsName);

	return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

bgfx::ProgramHandle loadProgram(const char* _vsName, const char* _fsName)
{
	return loadProgram(loco::entry::get_file_reader(), _vsName, _fsName);
}


#endif // BGFX_TEMP_H_HEADER_GUARD