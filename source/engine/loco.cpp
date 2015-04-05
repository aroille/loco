
#include "loco.h"
#include "log.h"
#include "platform.h"
#include "backend.h"
#include "resource_manager.h"

namespace loco
{
	size_t resource_root_path_length;
	char resource_root_path[LOCO_PATH_LENGTH];
	char default_resource_relativ_path[LOCO_PATH_LENGTH];
	
	EntityManager entity_manager;
	ResourceManager resource_manager;
	uint32 current_frame = 0;
	bool initialized = false;

	bool is_initialized() 
	{ 
		return initialized; 
	}

	void init(backend::Type renderer_type, int32 window_width, int32 window_height, const char* resources_path, const char* default_resources_path)
	{
		LOCO_LOG_INFO("Loco", "Initializing");
		LOCO_LOG_INFO("Loco", "Version %s %s %s %s", LOCO_CPU_NAME, LOCO_ARCH_NAME, LOCO_PLATFORM_NAME, LOCO_COMPILER_NAME);

		// init resources root path
		strcpy(resource_root_path, resources_path);
		resource_root_path_length = strlen(resource_root_path);

		strcpy(default_resource_relativ_path, default_resources_path);

		//init renderer
		backend::init(renderer_type);
		backend::reset(window_width, window_height);

		// load default resources
		resource_manager.load_folder(default_resource_relativ_path);
		resource_manager.init_default_resources();

		initialized = true;
	}

	void frame()
	{
		backend::frame();
		resource_manager.free_memory();
		current_frame++;
	}

	void shutdown()
	{
		resource_manager.unload_all();
		backend::shutdown();
	}

} // namespace loco