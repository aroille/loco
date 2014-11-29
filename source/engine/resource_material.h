#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

namespace loco
{
	
	template<> MaterialPtr ResourceManager::create(const Memory* mem) const
	{
		MaterialPtr mat_ptr = std::make_shared<Material>();
		Material& material = *mat_ptr.get();

		return mat_ptr;
	}
	
	template<> void ResourceManager::destroy(const MaterialPtr& material) const
	{

	}
	
}

#endif // RESOURCE_MATERIAL_H_HEADER_GUARD