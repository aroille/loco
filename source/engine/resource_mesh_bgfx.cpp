
#include <string.h> // strlen

#include <tinystl/allocator.h>
#include <tinystl/vector.h>
#include <tinystl/string.h>
namespace stl = tinystl;

#include <bgfx.h>
#include <bx/readerwriter.h>
#include <bx/fpumath.h>

#include "memory_utils.h"
#include "resource_type.h"
#include "loco.h"
#include "resource_manager.h"
#include "type.h"

struct Aabb
{
	float m_min[3];
	float m_max[3];
};

struct Obb
{
	float m_mtx[16];
};

struct Sphere
{
	float m_center[3];
	float m_radius;
};

struct Primitive
{
	uint32 m_startIndex;
	uint32 m_numIndices;
	uint32 m_startVertex;
	uint32 m_numVertices;

	Sphere m_sphere;
	Aabb m_aabb;
	Obb m_obb;
};

typedef stl::vector<Primitive> PrimitiveArray;

struct Group
{
	Group()
	{
		reset();
	}

	void reset()
	{
		m_vbh.idx = bgfx::invalidHandle;
		m_ibh.idx = bgfx::invalidHandle;
		m_prims.clear();
	}

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;
	Sphere m_sphere;
	Aabb m_aabb;
	Obb m_obb;
	PrimitiveArray m_prims;
};

namespace bgfx
{
	int32 read(bx::ReaderI* _reader, bgfx::VertexDecl& _decl);
}

namespace bgfx_helper
{


	struct Mesh
	{
		void load(bx::MemoryReader* _reader)
		{
#define BGFX_CHUNK_MAGIC_VB  BX_MAKEFOURCC('V', 'B', ' ', 0x1)
#define BGFX_CHUNK_MAGIC_IB  BX_MAKEFOURCC('I', 'B', ' ', 0x0)
#define BGFX_CHUNK_MAGIC_PRI BX_MAKEFOURCC('P', 'R', 'I', 0x0)

			using namespace bx;
			using namespace bgfx;

			Group group;

			uint32 chunk;
			while (4 == bx::read(_reader, chunk))
			{
				switch (chunk)
				{
					case BGFX_CHUNK_MAGIC_VB:
					{
						read(_reader, group.m_sphere);
						read(_reader, group.m_aabb);
						read(_reader, group.m_obb);

						read(_reader, m_decl);

						// convert vertex attribute declaration from bgfx to loco
						loco::Renderer::VertexDecl loco_decl;
						for (unsigned i = 0; i < (int32)loco::Renderer::Attrib::Count; i++)
						{
							loco::Renderer::Attrib::Enum type = (loco::Renderer::Attrib::Enum)i;
							if (m_decl.has((bgfx::Attrib::Enum)type))
							{
								loco::Renderer::VertexAttribDecl d;
								d.attrib = type;
			
								m_decl.decode((bgfx::Attrib::Enum)type, d.num, (bgfx::AttribType::Enum&)d.type, d.normalized, d.asInt);
								loco_decl.push_back(d);
							}
						}

						uint16 stride = m_decl.getStride();
						
						uint16 numVertices;
						read(_reader, numVertices);

						//const bgfx::Memory* mem = bgfx::alloc(numVertices*stride);
						//read(_reader, mem->data, mem->size);
						//group.m_vbh = bgfx::createVertexBuffer(mem, m_decl);
						loco::Memory mem;
						mem.data = const_cast<uint8*>(_reader->getDataPtr());
						mem.size = numVertices*stride;
						_reader->seek(mem.size, bx::Whence::Current);
						group.m_vbh.idx = loco::renderer.create_vertex_buffer(&mem, loco_decl).idx;
					}
					break;

					case BGFX_CHUNK_MAGIC_IB:
					{
						uint32 numIndices;
						read(_reader, numIndices);
						const bgfx::Memory* mem = bgfx::alloc(numIndices * 2);
						read(_reader, mem->data, mem->size);
						group.m_ibh = bgfx::createIndexBuffer(mem);
					}
					break;

					case BGFX_CHUNK_MAGIC_PRI:
					{
						uint16 len;
						read(_reader, len);

						stl::string material;
						material.resize(len);
						read(_reader, const_cast<char*>(material.c_str()), len);

						uint16 num;
						read(_reader, num);

						for (uint32 ii = 0; ii < num; ++ii)
						{
							read(_reader, len);

							stl::string name;
							name.resize(len);
							read(_reader, const_cast<char*>(name.c_str()), len);

							Primitive prim;
							read(_reader, prim.m_startIndex);
							read(_reader, prim.m_numIndices);
							read(_reader, prim.m_startVertex);
							read(_reader, prim.m_numVertices);
							read(_reader, prim.m_sphere);
							read(_reader, prim.m_aabb);
							read(_reader, prim.m_obb);

							group.m_prims.push_back(prim);
						}

						m_groups.push_back(group);
						group.reset();
					}
					break;

				default:
					bx::skip(_reader, 0);
					break;
				}
			}
		}

		bgfx::VertexDecl m_decl;
		typedef stl::vector<Group> GroupArray;
		GroupArray m_groups;
	};

	bool load_mesh(const loco::Memory* mem, loco::resource::MeshData* mesh)
	{
		bx::MemoryReader reader(mem->data, (uint32)mem->size);
		bgfx_helper::Mesh bgfx_mesh;
		bgfx_mesh.load(&reader);

		for (unsigned i = 0; i < bgfx_mesh.m_groups.size(); i++)
		{
			loco::resource::SubMeshData submesh;
			submesh.vertex_buffer = { bgfx_mesh.m_groups[i].m_vbh.idx };
			submesh.index_buffer = { bgfx_mesh.m_groups[i].m_ibh.idx };
			mesh->submeshes.push_back(submesh);
			mesh->materials.push_back(loco::resource_manager.get_default().material);
		}
		return true;
	}

} // bgfx_helper
