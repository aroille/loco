#ifndef RESOURCE_LOADER_H_HEADER_GUARD
#define RESOURCE_LOADER_H_HEADER_GUARD

#include "resource_manager.h"

namespace loco
{
	template<> ResourceManager::Texture ResourceManager::create(const Memory* mem) const
	{
		// Create hardware resource
		renderer::TextureHandle handle = renderer::create_texture(mem);

		// Create a new ResourceManager::Texture instance
		ResourceManager::Texture texture { handle };

		return texture;
	}

	template<> void ResourceManager::destroy(const ResourceManager::Texture& texture) const
	{
		renderer::destroy_texture(texture.handle);
	}
}

#endif // RESOURCE_LOADER_H_HEADER_GUARD