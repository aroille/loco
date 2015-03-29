#ifndef RESOURCE_TYPE_H_HEADER_GUARD
#define RESOURCE_TYPE_H_HEADER_GUARD

#include "murmur_hash.h" // HashedString
#include "renderer.h"
#include "type.h"

#include <list>
#include <map>
#include <memory>
#include <vector>

namespace loco{
namespace resource{

	/// Texture handle
	typedef Renderer::TextureHandle Texture;

	/// Shader handle
	typedef Renderer::ShaderHandle Shader;

	/// User interface to MaterialData
	/// 
	/// All creation/ modification / destruction of material have to be done using the Material interface.
	///
	/// MaterialData can be shared between several material instances.
	/// In this case, a modification of a material will modify all the shared materials
	/// \code
	/// Material a = b;
	/// a->set("tex_diffuse", tex_bricks); // modify a and b
	///	\endcode
	///
	/// @remarks
	/// Material is a shared_ptr<MaterialData>. \n
	/// MaterialData instance will be deleted automatically when no more Material are referencing this instance
	///
	class Material : public std::shared_ptr<MaterialData>
	{
	public:

		Material(MaterialData* mat = nullptr);

		static Material invalid;

		/// Create a new material from an existing one.
		/// In this case each materials has its own MaterialData
		/// \code
		/// Material a = b.duplicate();
		/// a->set("tex_diffuse", tex_bricks); // modify only the material a
		///	\endcode
		Material duplicate() const;
	};

	/// Describe a submesh geometry
	struct SubMeshData
	{
		Renderer::VertexBufferHandle vertex_buffer;
		Renderer::IndexBufferHandle index_buffer;
	};

	/// Describe a mesh (geometry and material for each submeshs)
	struct MeshData
	{
		std::vector<SubMeshData> submeshes;	///< Store the geometry description of each submesh
		std::vector<Material> materials;	///< Store the material of each submesh
	};

	/// User interface to MeshData
	/// 
	/// All creation/ modification / destruction of mesh have to be done using the Mesh interface.
	///
	/// MeshData can be shared between several mesh instances.
	/// In this case, a modification of a mesh will modify all the shared meshes
	/// \code
	/// Mesh a = b;
	/// a->submesh[0] = {vertex_buffer, index_buffer}; // modify the mesh a and b
	///	\endcode
	///
	/// @remarks
	/// Mesh is a shared_ptr<MeshData>. \n
	/// MeshData instance will be deleted automatically when no more Mesh are referencing this instance
	///
	class Mesh : public std::shared_ptr<MeshData>
	{
	public:
		Mesh(MeshData* mesh = nullptr);

		static Mesh invalid;

		/// Create a new mesh from an existing one.
		/// In this case each meshes has its own data
		/// \code
		/// Mesh b = a.duplicate();
		/// b->submesh[0] = {vertex_buffer, index_buffer}; // modify only the mesh b
		///	\endcode
		Mesh duplicate() const;
	};



	/// Describe a material by storing states/uniforms values  
	class MaterialData
	{
	public:

		struct UniformInfo
		{
			Renderer::UniformHandle				uniform;
			uint16												buffer_offset;
			uint16												array_size;
			Renderer::UniformType					type;
		};

		struct TextureInfo
		{
			Renderer::UniformHandle			uniform;
			Renderer::TextureHandle			texture;
			uint32											flags;
		};

		MaterialData();
		~MaterialData();

		void set_shader(Renderer::ShaderHandle vertex_shader, Renderer::ShaderHandle pixel_shader);

		void set(const char* name, Renderer::UniformType type, const float* data, unsigned size = 1);
		void set(const char* name, Renderer::TextureHandle texture, uint32 flags = 0);

		std::map<HashedString, unsigned> _uniform_map;
		std::map<HashedString, unsigned> _texture_map;

		Renderer::ProgramHandle		_program;
		std::vector<UniformInfo>	_uniform_infos;
		std::vector<TextureInfo>	_texture_infos;
		std::vector<float>				_uniform_buffer;

#ifdef LOCO_USE_HOT_RELOAD
		static std::list<MaterialData*> _all_materials;
#endif // LOCO_USE_HOT_RELOAD

	private:

		UniformInfo&  create_uniform_param(const char* name, Renderer::UniformType type, unsigned array_size);
		TextureInfo&  create_texture_param(const char* name);	

	};

	/// Default Resources
	struct DefaultResources
	{
		Shader		vertex_shader;
		Shader		pixel_shader;
		Texture		texture;
		Mesh			mesh;
		Material	material;
	};
} // resource
} // loco

#endif // RESOURCE_TYPE_H_HEADER_GUARD