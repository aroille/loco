#ifndef RESOURCE_MANAGER_H_HEADER_GUARD
#define RESOURCE_MANAGER_H_HEADER_GUARD

#include "murmur_hash.h"
#include "renderer.h"
#include <map>

namespace loco
{
	struct FileInfo;

	class ResourceManager
	{
	public:

		struct ResourceInfo
		{
			HashedString	hashed_name;
			Memory*			memory;
		};

		struct MeshResource
		{
			renderer::IndexBufferHandle vertex_buffer;
			renderer::IndexBufferHandle index_buffer;
		};

		struct TextureResource
		{
			renderer::TextureHandle texture;
		};

		struct Texture	{ HashedString name; };
		struct Mesh		{ HashedString name; };
		struct Shader	{ HashedString name; };
		struct Program	{ HashedString name; };

		/// Set the root folder path of this resource manager
		void init(const char* root_folder_path);

		/// Load all resources in path (recursive) + create the equivalent gpu resources
		/// Return the number of resource loaded by the function call
		unsigned load_folder(const char* folder_path);

		/// Return the resource handle associate with a specific resource path and resource type
		template<typename T> T get(const char* resource_path)
		{
			HashedString name = resource_name(resource_path);
			return T{ name };
		};

		/// Check if a specific resource is loaded
		/// Return true if the resource exist and is loaded, else return false
		template<typename T> bool is_loaded(T handle);



	private:

		char		_root_path[LOCO_PATH_LENGTH];
		unsigned	_root_path_lenght;
		std::map<HashedString, ResourceInfo> _resources;
		// ------
		std::map<HashedString, TextureResource>	_textures;
		std::map<HashedString, MeshResource>	_meshes;
		// ------
		ResourceInfo load_file(const FileInfo& fi);
		HashedString resource_name(const FileInfo& fi);
		HashedString resource_name(const char* resource_path);
		void create_texture(ResourceInfo ri);
	};


}

#endif // RESOURCE_MANAGER_H_HEADER_GUARD