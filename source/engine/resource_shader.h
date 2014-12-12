#ifndef RESOURCE_SHADER_H_HEADER_GUARD
#define RESOURCE_SHADER_H_HEADER_GUARD

namespace loco
{
	template<> ResourceManager::ResourceType::Enum ResourceManager::resource_type<Shader>() const
	{
		return ResourceType::Shader;
	}

	template<> std::map<ResourceName, Shader>& ResourceManager::resource_map()
	{ 
		return _shaders; 
	}

	template<> Shader ResourceManager::create(const Memory* mem) const
	{
		return renderer::create_shader(mem);
	}

	template<> Shader ResourceManager::replace(Shader& current, const Memory* mem) const
	{
		return current;
	}

	template<> void ResourceManager::destroy(const Shader& shader) const
	{
		renderer::destroy_shader(shader);
	}
}

#endif // RESOURCE_SHADER_H_HEADER_GUARD