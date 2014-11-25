#ifndef RESOURCE_TEXTURE_H_HEADER_GUARD
#define RESOURCE_TEXTURE_H_HEADER_GUARD

//#include "resource_manager.h"

namespace loco
{
	template<> Texture ResourceManager::create(const Memory* mem) const
	{
		// Create hardware resource
		renderer::TextureHandle handle = renderer::create_texture(mem);

		// Create a new ResourceManager::Texture instance
		Texture texture { handle };

		return texture;
	}

	template<> void ResourceManager::destroy(const Texture& texture) const
	{
		renderer::destroy_texture(texture.handle);
	}
}

#endif // RESOURCE_TEXTURE_H_HEADER_GUARD