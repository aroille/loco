
#ifndef BGFX_TEMP_H_HEADER_GUARD
#define BGFX_TEMP_H_HEADER_GUARD

#include "entry.h"
#include "input.h"
#include <bx/readerwriter.h>
#include <bx/fpumath.h>
#include "bgfx.h"
#include "cmd.h"

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


#define CAMERA_KEY_UP    UINT8_C(0x1)
#define CAMERA_KEY_DOWN  UINT8_C(0x2)
#define CAMERA_KEY_LEFT  UINT8_C(0x4)
#define CAMERA_KEY_RIGHT UINT8_C(0x8)

struct MouseCoords
{
	int32_t m_mx;
	int32_t m_my;
};

MouseCoords m_mouseNow, m_mouseLast;
float m_eye[3];
float m_at[3];
float m_up[3];
float m_horizontalAngle;
float m_verticalAngle;
float m_mouseSpeed;
float m_moveSpeed;
uint8_t m_keys = 0;
bool m_mouseDown;

void cameraReset()
{
	m_mouseNow.m_mx = 0;
	m_mouseNow.m_my = 0;
	m_mouseLast.m_mx = 0;
	m_mouseLast.m_my = 0;
	m_eye[0] = 0.0f;
	m_eye[1] = 0.0f;
	m_eye[2] = -35.0f;
	m_at[0] = 0.0f;
	m_at[1] = 0.0f;
	m_at[2] = -1.0f;
	m_up[0] = 0.0f;
	m_up[1] = 1.0f;
	m_up[2] = 0.0f;
	m_horizontalAngle = 0.01f;
	m_verticalAngle = 0.0f;
	m_mouseSpeed = 0.0020f;
	m_moveSpeed = 10.0f;
	m_keys = 0;
	m_mouseDown = false;
}

void cameraSetKeyState(uint8_t _key, bool _down)
{
	m_keys &= ~_key;
	m_keys |= _down ? _key : 0;
}

void cameraUpdate(float _deltaTime, uint32_t _mx, uint32_t _my, bool _move)
{
	if (!m_mouseDown)
	{
		m_mouseLast.m_mx = _mx;
		m_mouseLast.m_my = _my;
	}

	m_mouseDown = _move;

	if (_move)
	{
		m_mouseNow.m_mx = _mx;
		m_mouseNow.m_my = _my;
	}

	if (m_mouseDown)
	{
		int32_t deltaX = m_mouseNow.m_mx - m_mouseLast.m_mx;
		int32_t deltaY = m_mouseNow.m_my - m_mouseLast.m_my;

		m_horizontalAngle += m_mouseSpeed * float(deltaX);
		m_verticalAngle -= m_mouseSpeed * float(deltaY);

		m_mouseLast.m_mx = m_mouseNow.m_mx;
		m_mouseLast.m_my = m_mouseNow.m_my;
	}

	float direction[3] =
	{
		cosf(m_verticalAngle) * sinf(m_horizontalAngle),
		sinf(m_verticalAngle),
		cosf(m_verticalAngle) * cosf(m_horizontalAngle),
	};

	float right[3] =
	{
		sinf(m_horizontalAngle - bx::piHalf),
		0,
		cosf(m_horizontalAngle - bx::piHalf),
	};

	if (m_keys & CAMERA_KEY_UP)
	{
		// m_eye += direction * _deltaTime * m_moveSpeed
		float tmpRhs[3];
		float tmpPos[3];
		memcpy(tmpPos, m_eye, sizeof(float)* 3);
		bx::vec3Mul(tmpRhs, direction, _deltaTime * m_moveSpeed);
		bx::vec3Add(m_eye, tmpPos, tmpRhs);
		cameraSetKeyState(CAMERA_KEY_UP, false);
	}

	if (m_keys & CAMERA_KEY_DOWN)
	{
		// m_eye -= direction * _deltaTime * m_moveSpeed
		float tmpRhs[3];
		float tmpPos[3];
		memcpy(tmpPos, m_eye, sizeof(float)* 3);
		bx::vec3Mul(tmpRhs, direction, _deltaTime * m_moveSpeed);
		bx::vec3Sub(m_eye, tmpPos, tmpRhs);
		cameraSetKeyState(CAMERA_KEY_DOWN, false);
	}

	if (m_keys & CAMERA_KEY_LEFT)
	{
		// m_eye += right * _deltaTime * m_moveSpeed
		float tmpRhs[3];
		float tmpPos[3];
		memcpy(tmpPos, m_eye, sizeof(float)* 3);
		bx::vec3Mul(tmpRhs, right, _deltaTime * m_moveSpeed);
		bx::vec3Add(m_eye, tmpPos, tmpRhs);
		cameraSetKeyState(CAMERA_KEY_LEFT, false);
	}

	if (m_keys & CAMERA_KEY_RIGHT)
	{
		// m_eye -= right * _deltaTime * m_moveSpeed
		float tmpRhs[3];
		float tmpPos[3];
		memcpy(tmpPos, m_eye, sizeof(float)* 3);
		bx::vec3Mul(tmpRhs, right, _deltaTime * m_moveSpeed);
		bx::vec3Sub(m_eye, tmpPos, tmpRhs);
		cameraSetKeyState(CAMERA_KEY_RIGHT, false);
	}

	bx::vec3Add(m_at, m_eye, direction);
	bx::vec3Cross(m_up, right, direction);
}

int cmdMove(loco::CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	if (_argc > 1)
	{
		if (0 == strcmp(_argv[1], "forward"))
		{
			cameraSetKeyState(CAMERA_KEY_UP, true);
			return 0;
		}
		else if (0 == strcmp(_argv[1], "left"))
		{
			cameraSetKeyState(CAMERA_KEY_LEFT, true);
			return 0;
		}
		else if (0 == strcmp(_argv[1], "right"))
		{
			cameraSetKeyState(CAMERA_KEY_RIGHT, true);
			return 0;
		}
		else if (0 == strcmp(_argv[1], "backward"))
		{
			cameraSetKeyState(CAMERA_KEY_DOWN, true);
			return 0;
		}
	}

	return 1;
}

static void cmd(const void* _userData)
{
	loco::cmdExec((const char*)_userData);
}

static const loco::InputBinding s_camBindings[] =
{
	{ loco::entry::Key::KeyW, loco::entry::Modifier::None, 0, cmd, "move forward" },
	{ loco::entry::Key::KeyA, loco::entry::Modifier::None, 0, cmd, "move left" },
	{ loco::entry::Key::KeyS, loco::entry::Modifier::None, 0, cmd, "move backward" },
	{ loco::entry::Key::KeyD, loco::entry::Modifier::None, 0, cmd, "move right" },

	INPUT_BINDING_END
};






#endif // BGFX_TEMP_H_HEADER_GUARD