#ifndef RESOURCE_LOADER_H_HEADER_GUARD
#define RESOURCE_LOADER_H_HEADER_GUARD

#include "resource_manager.h"

namespace loco
{
	ResourceManager::TextureResource load_texture(ResourceManager::ResourceInfo ri)
	{
		// Create hardware resource
		renderer::TextureHandle texture_handle = renderer::create_texture(ri.memory);

		// Create a new ResourceManager::TextureResource instance
		ResourceManager::TextureResource texture_resource;
		texture_resource.texture = texture_handle;

		return texture_resource;
	}
}

#endif // RESOURCE_LOADER_H_HEADER_GUARD