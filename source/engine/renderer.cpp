#include "renderer.h"
#include "memory_utils.h"
#include "log.h"
#include "resource_type.h"

#include "bgfx.h" // backend

#define LOCO_RENDERER "Renderer" // log module string

#define LOCO_TO_BGFX(handle) { handle.idx }
#define BGFX_TO_LOCO(handle) { handle.idx }

#define LOCO_RENDERER_HANDLE_DEF(_name) \
bool Renderer::_name::operator==(_name const& in) const \
{ return idx == in.idx; }; \
Renderer::_name Renderer::_name::invalid = { UINT16_MAX };

namespace loco
{
	using namespace resource;
	using namespace math;

	LOCO_RENDERER_HANDLE_DEF(TextureHandle);
	LOCO_RENDERER_HANDLE_DEF(VertexDeclHandle);
	LOCO_RENDERER_HANDLE_DEF(VertexBufferHandle);
	LOCO_RENDERER_HANDLE_DEF(IndexBufferHandle);
	LOCO_RENDERER_HANDLE_DEF(ShaderHandle);
	LOCO_RENDERER_HANDLE_DEF(ProgramHandle);
	LOCO_RENDERER_HANDLE_DEF(UniformHandle);

	char shader_extentions[(uint32)Renderer::Type::Count][16] =
	{
		"shader_none",
		"shader_dx9",
		"shader_dx11",
		"shader_gles",
		"shader_glsl"
	};

	bgfx::UniformType::Enum UniformType_convert[(uint32)Renderer::UniformType::Count] =
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
	void Renderer::init(Renderer::Type renderer_type)
	{
		LOCO_LOG_INFO(LOCO_RENDERER, "Initializing");
		bgfx::init((bgfx::RendererType::Enum)renderer_type);
		LOCO_LOG_INFO(LOCO_RENDERER, "%s", bgfx::getRendererName(bgfx::getRendererType()));
		strcpy(_shader_extention, shader_extentions[(uint32)type()]);
	}

	//==========================================================================
	void Renderer::reset(unsigned width, unsigned height)
	{
		uint32 reset = BGFX_RESET_VSYNC;

		bgfx::reset(width, height, reset);
	}

	//==========================================================================
	void Renderer::shutdown()
	{
		bgfx::shutdown();
	}

	//==========================================================================
	Renderer::Type Renderer::type()
	{
		return (Type)bgfx::getRendererType();
	}

	//==========================================================================
	const char* Renderer::type_name(Type type)
	{
		return bgfx::getRendererName((bgfx::RendererType::Enum) type);
	}

	//==========================================================================
	void Renderer::frame()
	{
		bgfx::frame();
	}

	//==========================================================================
	Renderer::TextureHandle Renderer::create_texture(const Memory* memory)
	{
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
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

		uint32 key = vsh.idx + (psh.idx << 16);
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
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
		return BGFX_TO_LOCO(bgfx::createShader(bgfx_mem));
	}

	//==========================================================================
	void Renderer::destroy_shader(ShaderHandle handle)
	{
		bgfx::destroyShader(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	Renderer::UniformHandle Renderer::create_uniform(const char* name, UniformType type, unsigned array_size)
	{
		bgfx::UniformHandle bgfx_handle = bgfx::createUniform(name, UniformType_convert[(uint32)type], array_size);
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

		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
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
		const bgfx::Memory* bgfx_mem = bgfx::makeRef(memory->data, (uint32)memory->size);
		return BGFX_TO_LOCO(bgfx::createIndexBuffer(bgfx_mem));
	}

	//==========================================================================
	void Renderer::destroy_index_buffer(IndexBufferHandle handle)
	{
		bgfx::destroyIndexBuffer(LOCO_TO_BGFX(handle));
	}

	//==========================================================================
	void Renderer::set_view_rect(uint8 view_id, Viewport viewport)
	{
		bgfx::setViewRect(view_id, viewport.x, viewport.y, viewport.width, viewport.height);
	}

	//==========================================================================
	void Renderer::set_view_transform(uint8 view_id, const Matrix4x4& view, const Matrix4x4& proj)
	{
		bgfx::setViewTransform(view_id, (const void*)view.m, (const void*)proj.m);
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
			uint8 tex_unit = 0;
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
	void Renderer::batch_render(uint8 view_id, uint32 count, const Mesh* meshes, const Matrix4x4* transforms, const DefaultResources& default_resources)
	{
		for (unsigned i = 0; i < count; i++)
		{
			const Mesh& m = (*meshes == Mesh::invalid) ? default_resources.mesh : *meshes;
			const void* world_matrix = (void*)transforms;

			for (unsigned j = 0; j < m->submeshes.size(); j++)
			{
				// set material
				bind_material(m->materials[j].get(), default_resources);

				// set model matrix
				bgfx::setTransform(world_matrix);

				// set vertex and index buffer
				bgfx::setVertexBuffer(bgfx::VertexBufferHandle LOCO_TO_BGFX(m->submeshes[j].vertex_buffer));
				bgfx::setIndexBuffer(bgfx::IndexBufferHandle LOCO_TO_BGFX(m->submeshes[j].index_buffer));

				// submit draw call
				bgfx::submit(view_id);
			}

			meshes++;
			transforms++;
		}
	}
} // loco
