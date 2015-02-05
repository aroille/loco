#ifndef RESOURCE_TEXTURE_H_HEADER_GUARD
#define RESOURCE_TEXTURE_H_HEADER_GUARD

namespace loco{
namespace resource{

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
		return renderer.create_texture(mem);
	}

	template<> void ResourceManager::destroy(const Texture& texture) const
	{
		renderer.destroy_texture(texture);
	}

#ifdef LOCO_USE_HOT_RELOAD
	template<> Texture ResourceManager::replace(Texture& current, const Memory* mem) const
	{
		Texture new_texture = renderer.create_texture(mem);

		if (new_texture == Texture::invalid)
		{
			return current;
		}
		else
		{
			renderer.destroy_texture(current);

			auto mat_iter = MaterialData::_all_materials.begin();
			auto mat_iter_end = MaterialData::_all_materials.end();
			for (mat_iter; mat_iter != mat_iter_end; ++mat_iter)
			{
				auto tex_iter = (*mat_iter)->_texture_infos.begin();
				auto tex_iter_end = (*mat_iter)->_texture_infos.end();
				for (tex_iter; tex_iter != tex_iter_end; ++tex_iter)
				{
					if ((*tex_iter).texture == current)
						(*tex_iter).texture = new_texture;
				}
			}

			return new_texture;
		}

	}
#endif // LOCO_USE_HOT_RELOAD

} // resource
} // loco

#endif // RESOURCE_TEXTURE_H_HEADER_GUARD