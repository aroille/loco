#include "renderer.h"
#include "bgfx.h"
#include "memory_utils.h"
#include "resource_manager.h"
#include "loco.h"

#define LOCO_TO_BGFX(handle) { handle.idx }
#define BGFX_TO_LOCO(handle) { handle.idx }

#define LOCO_RENDERER_HANDLE_DEF(_name) \
bool Renderer::_name::operator==(_name const& in) const \
{ return idx == in.idx; }; \
Renderer::_name Renderer::_name::invalid = { UINT16_MAX };

namespace loco
{
	using namespace resource;

	LOCO_RENDERER_HANDLE_DEF(TextureHandle);
	LOCO_RENDERER_HANDLE_DEF(VertexDeclHandle);
	LOCO_RENDERER_HANDLE_DEF(VertexBufferHandle);
	LOCO_RENDERER_HANDLE_DEF(IndexBufferHandle);
	LOCO_RENDERER_HANDLE_DEF(ShaderHandle);
	LOCO_RENDERER_HANDLE_DEF(ProgramHandle);
	LOCO_RENDERER_HANDLE_DEF(UniformHandle);

	char shader_extentions[Renderer::Type::Count][16] =
	{
		"shader_none",
		"shader_dx9",
		"shader_dx11",
		"shader_gles",
		"shader_glsl"
	};

	bgfx::UniformType::Enum UniformType_convert[Renderer::UniformType::Count] =
	{
		bgfx::UniformType::Uniform1fv,
		bgfx::UniformType::Uniform2fv,
		bgfx::UniformType::Uniform3fv,
		bgfx::UniformType::Uniform4fv,
		bgfx::UniformType::Uniform3x3fv,
		bgfx::UniformType::Uniform4x4fv,
		bgfx::UniformType::Uniform1iv
	};

	//==========================================================================
	void Renderer::init()
	{
		log.info(LOCO_LOG_RENDERER, "Initializing");
		bgfx::init();
		log.info(LOCO_LOG_RENDERER, "%s", bgfx::getRendererName(bgfx::getRendererType()));
		strcpy(_shader_extention, shader_extentions[type()]);
	}

	//==========================================================================
	void Renderer::reset(unsigned width, unsigned height)
	{
		uint32_t reset = BGFX_RESET_VSYNC;

		bgfx::reset(width, height, reset);
	}

	//==========================================================================
	void Renderer::shutdown()
	{
		bgfx::shutdown();
	}

	//==========================================================================
	Renderer::Type::Enum Renderer::type()
	{
		return (Type::Enum)bgfx::getRendererType();
	}

	//==========================================================================
	const char* Renderer::type_name(Type::Enum type)
	{
		return bgfx::getRendererName((bgfx::RendererType::Enum) type);
	}

	//==========================================================================
	Renderer::TextureHandle Renderer::create_texture(const Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		return BGFX_TO_LOCO(bgfx::createTexture(bgfx_mem));
	}

	//==========================================================================
	void Renderer::destroy_texture(TextureHandle handle)
	{
		bgfx::destroyTexture(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	Renderer::ProgramHandle Renderer::create_program(ShaderHandle vsh, ShaderHandle psh)
	{
		ProgramHandle handle;

		uint32_t key = vsh.idx + (psh.idx << 16);
		auto it = _programs.find(key);
		if (it != _programs.end())
		{
			handle = it->second;
		}
		else
		{
			handle = BGFX_TO_LOCO(bgfx::createProgram(LOCO_TO_BGFX(vsh), LOCO_TO_BGFX(psh), false));
			_programs[key] = handle;
		}

		return handle;
	}

	//==========================================================================
	void Renderer::destroy_program(ProgramHandle handle)
	{
		bgfx::destroyProgram(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	Renderer::ShaderHandle Renderer::create_shader(const Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		return BGFX_TO_LOCO(bgfx::createShader(bgfx_mem));
	}

	//==========================================================================
	void Renderer::destroy_shader(ShaderHandle handle)
	{
		bgfx::destroyShader(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	Renderer::UniformHandle Renderer::create_uniform(const char* name, UniformType::Enum type, unsigned array_size)
	{
		bgfx::UniformHandle bgfx_handle = bgfx::createUniform(name, UniformType_convert[type], array_size);
		return BGFX_TO_LOCO(bgfx_handle);
	}

	//==========================================================================
	void Renderer::destroy_uniform(UniformHandle handle)
	{
		bgfx::destroyUniform(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	Renderer::VertexBufferHandle Renderer::create_vertex_buffer(const Memory* memory, const VertexDecl& decl)
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
	void Renderer::destroy_vertex_buffer(VertexBufferHandle handle)
	{
		bgfx::destroyVertexBuffer(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	Renderer::IndexBufferHandle Renderer::create_index_buffer(const Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, memory->size);
		return BGFX_TO_LOCO(bgfx::createIndexBuffer(bgfx_mem));
	}

	//==========================================================================
	void Renderer::destroy_index_buffer(IndexBufferHandle handle)
	{
		bgfx::destroyIndexBuffer(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	void Renderer::bind_material(const MaterialData* material, const DefaultResources& default_resources)
	{
		const MaterialData* m = (material == nullptr) ? default_resources.material.get() : material;

		// programs
		bgfx::setProgram(LOCO_TO_BGFX(m->_program));

		// numeric parameters
		{
			std::vector<MaterialData::UniformInfo>::const_iterator it = m->_uniform_infos.cbegin();
			while (it != m->_uniform_infos.cend())
			{
				bgfx::setUniform(LOCO_TO_BGFX((*it).uniform), m->_uniform_buffer.data() + (*it).buffer_offset, (*it).array_size);
				it++;
			}
		}

		// samplers
		{
			uint8_t tex_unit = 0;
			std::vector<MaterialData::TextureInfo>::const_iterator it = m->_texture_infos.cbegin();
			while (it != m->_texture_infos.cend())
			{
				const Texture& texture = ((*it).texture == Texture::invalid) ? default_resources.texture : (*it).texture;
				bgfx::setTexture(tex_unit, LOCO_TO_BGFX((*it).uniform), LOCO_TO_BGFX(texture), (*it).flags);
				tex_unit++;
				it++;
			}
		}

		// render states
		bgfx::setState(0 \
			| BGFX_STATE_RGB_WRITE \
			| BGFX_STATE_ALPHA_WRITE \
			| BGFX_STATE_DEPTH_TEST_LESS \
			| BGFX_STATE_DEPTH_WRITE \
			| BGFX_STATE_CULL_CCW \
			| BGFX_STATE_MSAA \
			);
	}

	//==========================================================================
	void Renderer::submit(uint8_t view_id, const Mesh& mesh, const MaterialData* material, const void* model_matrix, const DefaultResources& default_resources)
	{
		const Mesh& m = (mesh == Mesh::invalid) ? default_resources.mesh : mesh;

		for (unsigned i = 0; i < m->submeshes.size(); i++)
		{
			// set material
			bind_material(material, default_resources);

			// set model matrix
			bgfx::setTransform(model_matrix);

			// set vertex and index buffer
			bgfx::setVertexBuffer(bgfx::VertexBufferHandle LOCO_TO_BGFX(m->submeshes[i].vertex_buffer));
			bgfx::setIndexBuffer(bgfx::IndexBufferHandle LOCO_TO_BGFX(m->submeshes[i].index_buffer));
			
			// submit draw call
			bgfx::submit(view_id);
		}
	}
} // loco
