#include "renderer.h"
#include "bgfx.h"
#include "memory_utils.h"
#include "resource_material.h"
#include "resource_manager.h"

#define LOCO_TO_BGFX(handle) { handle.idx }
#define BGFX_TO_LOCO(handle) { handle.idx }

namespace loco
{
namespace renderer
{
	TextureHandle TextureHandle::invalid = { 0 };
	ShaderHandle ShaderHandle::invalid = { 0 };
	ProgramHandle ProgramHandle::invalid = { 0 };

	bgfx::UniformType::Enum UniformType_convert[UniformType::Count] =
	{
		bgfx::UniformType::Uniform1fv,
		bgfx::UniformType::Uniform2fv,
		bgfx::UniformType::Uniform3fv,
		bgfx::UniformType::Uniform4fv,
		bgfx::UniformType::Uniform3x3fv,
		bgfx::UniformType::Uniform4x4fv,
		bgfx::UniformType::Uniform1iv
	};

	typedef std::map<uint32_t, ProgramHandle> ProgramMap;
	ProgramMap _programs;

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
		return BGFX_TO_LOCO(bgfx::createTexture(bgfx_mem));
	}

	//==========================================================================
	void destroy_texture(TextureHandle handle)
	{
		bgfx::destroyTexture(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	ProgramHandle create_program(ShaderHandle vsh, ShaderHandle fsh)
	{
		ProgramHandle handle;

		uint32_t key = vsh.idx + (fsh.idx << 16);
		auto it = _programs.find(key);
		if (it != _programs.end())
		{
			handle = it->second;
		}
		else
		{
			handle = BGFX_TO_LOCO(bgfx::createProgram(LOCO_TO_BGFX(vsh), LOCO_TO_BGFX(fsh), false));
			_programs[key] = handle;
		}

		return handle;
	}

	//==========================================================================
	void destroy_program(ProgramHandle handle)
	{
		bgfx::destroyProgram(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	ShaderHandle create_shader(const Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		return BGFX_TO_LOCO(bgfx::createShader(bgfx_mem));
	}

	//==========================================================================
	void destroy_shader(ShaderHandle handle)
	{
		bgfx::destroyShader(LOCO_TO_BGFX(handle));
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

	//==========================================================================
	VertexBufferHandle create_vertex_buffer(const Memory* memory, const VertexDecl& decl)
	{
		bgfx::VertexDecl bgfx_decl;
		bgfx_decl.begin();
		for (unsigned i = 0; i < decl.size(); i++)
		{
			const VertexAttribDecl& d = decl[i];
			bgfx_decl.add((bgfx::Attrib::Enum)d.attrib, d.num, (bgfx::AttribType::Enum)d.type, d.normalized, d.asInt);
		}
		bgfx_decl.end();

		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		return BGFX_TO_LOCO(bgfx::createVertexBuffer(bgfx_mem, bgfx_decl));
	}

	//==========================================================================
	VertexBufferHandle create_vertex_buffer(const Memory* memory, const bgfx::VertexDecl& bgfx_decl)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		return BGFX_TO_LOCO(bgfx::createVertexBuffer(bgfx_mem, bgfx_decl));
	}

	//==========================================================================
	void destroy_vertex_buffer(VertexBufferHandle handle)
	{
		bgfx::destroyVertexBuffer(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	IndexBufferHandle create_index_buffer(const Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		return BGFX_TO_LOCO(bgfx::createIndexBuffer(bgfx_mem));
	}

	//==========================================================================
	void destroy_index_buffer(IndexBufferHandle handle)
	{
		bgfx::destroyIndexBuffer(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	void bind_material(const Material& mat)
	{
		// programs
		bgfx::setProgram(LOCO_TO_BGFX(mat._program));

		// numeric parameters
		{
			std::vector<Material::UniformInfo>::const_iterator it = mat._uniform_infos.cbegin();
			while (it != mat._uniform_infos.cend())
			{
				bgfx::setUniform(LOCO_TO_BGFX((*it).uniform), mat._uniform_buffer.data() + (*it).buffer_offset, (*it).buffer_offset);
				it++;
			}
		}

		// samplers
		{
			uint8_t tex_unit = 0;
			std::vector<Material::TextureInfo>::const_iterator it = mat._texture_infos.cbegin();
			while (it != mat._texture_infos.cend())
			{
				bgfx::setTexture(tex_unit, LOCO_TO_BGFX((*it).uniform), LOCO_TO_BGFX((*it).texture), (*it).flags);
				tex_unit++;
				it++;
			}
		}

		// render states
		bgfx::setState(BGFX_STATE_DEFAULT);
	}

	void submit(uint8_t view_id, const Mesh& mesh, const Material& mat, const void* model_matrix)
	{
		for (unsigned i = 0; i < mesh.submeshes.size(); i++)
		{
			// set material
			bind_material(mat);

			// set model matrix
			bgfx::setTransform(model_matrix);

			// set vertex and index buffer
			bgfx::setVertexBuffer(bgfx::VertexBufferHandle LOCO_TO_BGFX(mesh.submeshes[i].vertex_buffer));
			bgfx::setIndexBuffer(bgfx::IndexBufferHandle LOCO_TO_BGFX(mesh.submeshes[i].index_buffer));
			
			// submit draw call
			bgfx::submit(view_id);
		}
	}

} // renderer
} // loco
