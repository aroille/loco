#ifndef RESOURCE_TEXTURE_H_HEADER_GUARD
#define RESOURCE_TEXTURE_H_HEADER_GUARD

namespace loco
{
	template<> Texture ResourceManager::create(const Memory* mem) const
	{
		return renderer::create_texture(mem);
	}

	template<> void ResourceManager::destroy(const Texture& texture) const
	{
		renderer::destroy_texture(texture);
	}
}

#endif // RESOURCE_TEXTURE_H_HEADER_GUARD