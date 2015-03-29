#ifndef RESOURCE_SHADER_H_HEADER_GUARD
#define RESOURCE_SHADER_H_HEADER_GUARD

namespace loco {
namespace resource {

	template<> ResourceManager::ResourceType ResourceManager::resource_type<Shader>() const
	{
		return ResourceType::Shader;
	}

	template<> const char* ResourceManager::resource_type_name<Shader>() const
	{
		return "Shader";
	}

	template<> std::map<ResourceName, Shader>& ResourceManager::resource_map()
	{
		return _shaders;
	}

	template<> const std::map<ResourceName, Shader>& ResourceManager::resource_map() const
	{
		return _shaders;
	}

	template<> Shader ResourceManager::create(const Memory* mem) const
	{
		return renderer.create_shader(mem);
	}

#ifdef LOCO_USE_HOT_RELOAD
	template<> Shader ResourceManager::replace(Shader& current, const Memory* mem) const
	{
		if (mem->size == 0)
			return current;

		Shader new_shader = renderer.create_shader(mem);
		renderer.destroy_shader(current);
		return new_shader;
	}
#endif // LOCO_USE_HOT_RELOAD

	template<> void ResourceManager::destroy(const Shader& shader) const
	{
		renderer.destroy_shader(shader);
	}
} // resource
} // loco

#endif // RESOURCE_SHADER_H_HEADER_GUARD