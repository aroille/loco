#ifndef RESOURCE_TEXTURE_H_HEADER_GUARD
#define RESOURCE_TEXTURE_H_HEADER_GUARD

namespace loco
{
	template<> ResourceManager::ResourceType::Enum ResourceManager::resource_type<Texture>() const
	{
		return ResourceType::Texture;
	}

	template<> const char* ResourceManager::resource_type_name<Texture>() const
	{
		return "Texture";
	}

	template<> std::map<ResourceName, Texture>& ResourceManager::resource_map()
	{ 
		return _textures; 
	}

	template<> Texture ResourceManager::create(const Memory* mem) const
	{
		return renderer::create_texture(mem);
	}

	template<> Texture ResourceManager::replace(Texture& current, const Memory* mem) const
	{
		return current;
	}

	template<> void ResourceManager::destroy(const Texture& texture) const
	{
		renderer::destroy_texture(texture);
	}
}

#endif // RESOURCE_TEXTURE_H_HEADER_GUARD