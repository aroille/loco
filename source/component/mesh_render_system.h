#ifndef MESH_RENDER_SYSTEM_H_HEADER_GUARD
#define MESH_RENDER_SYSTEM_H_HEADER_GUARD

#include "entity.h"
#include "handle.h"
#include "math_types.h"
#include "resource_type.h"
#include "system_common.h"

#include <unordered_map>

namespace loco
{
	using namespace math;

	struct World;

	struct MeshRenderComponent
	{
		HandleI24G8 handle;
		static const MeshRenderComponent null;
	};

	/// Manager of mesh render components
	/// (one meshRenderSystem per world)
	class MeshRenderSystem
	{
	public:

		MeshRenderSystem(World* world, TransformSyncCallback transform_sync_callback);
		~MeshRenderSystem();

		/// Create a mesh render component
		///
		/// @remarks 
		/// Only one mesh render component per entity is allowed 
		///
		MeshRenderComponent create(Entity e);

		/// Get the mesh render component attach to an entity
		MeshRenderComponent lookup(Entity e);

		/// Check if component c is valid
		bool is_valid(MeshRenderComponent c);

		/// Free a mesh render component
		void destroy(Entity e);

		/// Set Mesh
		void set_mesh(MeshRenderComponent c, const Mesh& mesh);

		/// Synchronize transforms from TransformSystem data
		void sync_transform(unsigned count, Entity* entity, Matrix4x4* transform);


		struct ComponentData
		{
			unsigned size;				///< Number of used instance
			unsigned capacity;			///< Number of allocated instance in arrays
			char* buffer;				///< Buffer with instance data

			Matrix4x4*	transform;		///< World transform
			Mesh*		mesh;			///< Mesh
			MeshRenderComponent*	component;		///< Owner component

			unsigned* lut;				///< Look up table : Component/Data_index 
		};
		ComponentData _data;

	private:

		/// Handle manager (Component = handle)
		HandleManagerI24G8	_handle_mgr;

		/// World owning this system
		World* _world;

		TransformSyncCallback _transform_sync_callback;

		/// Map an Entity (key) with a Component (value)
		std::unordered_map<unsigned, MeshRenderComponent> _map;

		/// Return a _data index from a component
		inline unsigned data_index(MeshRenderComponent c)
		{
			return _data.lut[c.handle.index()];
		};

		/// Increase the capicity of the data buffer to an component count of sz
		void allocate(unsigned sz);

		/// Move a component at index 'from' to a new location at index 'to'
		/// The memory at index 'to' shoudn't be used by another component before the move;
		void move_instance(unsigned from, unsigned to);
	};
}

#endif // MESH_RENDER_SYSTEM_H_HEADER_GUARD