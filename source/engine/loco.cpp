
#include "loco.h"
#include "platform.h"
#include "resource_manager.h"
#include "renderer.h"

namespace loco
{
	size_t resource_root_path_length;
	char resource_root_path[LOCO_PATH_LENGTH];
	char default_resource_relativ_path[LOCO_PATH_LENGTH];
	
	EntityManager entity_manager;
	ResourceManager resource_manager;
	Renderer renderer;
	Log log;
	uint32 current_frame = 0;
	bool initialized = false;

	bool is_initialized() 
	{ 
		return initialized; 
	}

	void init(Renderer::Type::Enum renderer_type, int32 window_width, int32 window_height, const char* resources_path, const char* default_resources_path)
	{
		log.info("Loco", "Initializing");
		log.info("Loco", "Version %s %s %s %s", LOCO_CPU_NAME, LOCO_ARCH_NAME, LOCO_PLATFORM_NAME, LOCO_COMPILER_NAME);

		// init resources root path
		strcpy(resource_root_path, resources_path);
		resource_root_path_length = strlen(resource_root_path);

		strcpy(default_resource_relativ_path, default_resources_path);

		//init renderer
		renderer.init(renderer_type);
		renderer.reset(window_width, window_height);

		// load default resources
		resource_manager.load_folder(default_resource_relativ_path);
		resource_manager.init_default_resources();

		initialized = true;
	}

	void frame()
	{
		renderer.frame();
		resource_manager.free_memory();
		current_frame++;
	}

	void shutdown()
	{
		resource_manager.unload_all();
		renderer.shutdown();
	}

} // namespace loco