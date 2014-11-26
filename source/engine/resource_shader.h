#ifndef RESOURCE_SHADER_H_HEADER_GUARD
#define RESOURCE_SHADER_H_HEADER_GUARD

namespace loco
{
	template<> Shader ResourceManager::create(const Memory* mem) const
	{
		return renderer::create_shader(mem);
	}

	template<> void ResourceManager::destroy(const Shader& shader) const
	{
		renderer::destroy_shader(shader);
	}
}

#endif // RESOURCE_SHADER_H_HEADER_GUARD