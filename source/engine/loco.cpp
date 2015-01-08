
#include "loco.h"
#include "renderer.h"

namespace loco
{
	unsigned resource_root_path_length;
	char resource_root_path[LOCO_PATH_LENGTH];
	char default_resource_relativ_path[LOCO_PATH_LENGTH];
	
	EntityManager entity_manager;
	ResourceManager resource_manager;
	Renderer renderer;
	Log log;
	
	void init(const char* resources_path, const char* default_resources_path)
	{
		log.info(LOCO_LOG, "Initializing");

		// init resources root path
		strcpy(resource_root_path, resources_path);
		resource_root_path_length = strlen(resource_root_path);

		strcpy(default_resource_relativ_path, default_resources_path);

		//init renderer
		renderer.init();
		renderer.reset(1280, 720);

		// load default resources
		loco::resource_manager.load_folder(default_resource_relativ_path);
		loco::resource_manager.init_default_resources();
	}

	void shutdown()
	{
		resource_manager.unload_all();
		renderer.shutdown();
	}

	void render(const World& world)
	{
		MeshRenderSystem::ComponentData data = world.mesh_renders._data;
		renderer.batch_render(0, data.size, data.mesh, data.transform, resource_manager.get_default());
	}

} // namespace loco