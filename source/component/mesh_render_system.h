#ifndef MESH_RENDER_SYSTEM_H_HEADER_GUARD
#define MESH_RENDER_SYSTEM_H_HEADER_GUARD

#include "entity.h"
#include "math_types.h"
#include "resource_type.h"

namespace loco
{
	/// Manager of mesh render components
	class MeshRenderSystem
	{
	public:

		/// MeshRenderer component (just a handle)
		struct Component
		{
			unsigned i;
		};

		/// Add a mesh renderer component to an entity
		///
		/// @remarks 
		/// Only one mesh renderer component per entity is allowed 
		///
		//Component add(Entity e);

		/// Get the mesh renderer component attach to an entity
		//Component lookup(Entity e);

		/// Remove a mesh renderer component from an entity
		//void remove(Entity e);

		/// Get number of submesh of a mesh


	private:

		struct ComponentData
		{
			unsigned size;				///< Number of used instance
			unsigned capacity;			///< Number of allocated instance in arrays
			char* buffer;				///< Buffer with instance data

			SubMesh*		submesh;		///< Submesh
			Material*		material;		///< material (shared pointer)
			Matrix4x4*		transform;		///< World transform
			Entity*			entity;			///< The entity owning this instance


		};
		ComponentData _data;


	};
}

#endif // MESH_RENDER_SYSTEM_H_HEADER_GUARD