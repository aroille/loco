#ifndef MESH_RENDER_SYSTEM_H_HEADER_GUARD
#define MESH_RENDER_SYSTEM_H_HEADER_GUARD

#include "entity.h"
#include "handle.h"
#include "math_types.h"
#include "resource_type.h"
#include <unordered_map>

namespace loco
{
	using namespace math;

	/// Manager of mesh render components
	class MeshRenderSystem
	{
	public:

		/// MeshRender component (just a handle)
		typedef HandleI24G8 Component;

		MeshRenderSystem();
		~MeshRenderSystem();

		/// Create a mesh render component
		///
		/// @remarks 
		/// Only one mesh render component per entity is allowed 
		///
		Component create(Entity e);

		/// Get the mesh render component attach to an entity
		Component lookup(Entity e);

		/// Check if component c is valid
		bool is_valid(Component c);

		/// Free a mesh render component
		void destroy(Entity e);

		/// Set Mesh
		void set_mesh(Component c, const Mesh& mesh);

		struct ComponentData
		{
			unsigned size;				///< Number of used instance
			unsigned capacity;			///< Number of allocated instance in arrays
			char* buffer;				///< Buffer with instance data

			Matrix4x4*	transform;		///< World transform
			Mesh*		mesh;			///< Mesh
			Component*	component;		///< Owner component

			unsigned* lut;				///< Look up table : Component/DataIndex 
		};
		ComponentData _data;

	private:

		/// Handle manager (Component = handle)
		HandleManagerI24G8	_handle_mgr;

		/// Map an Entity (key) with a Component (value)
		std::unordered_map<unsigned, Component> _map;

		/// Return a _data index from a component
		inline unsigned data_index(Component c)
		{
			return _data.lut[c.index()];
		};

		/// Increase the capicity of the data buffer to an component count of sz
		void allocate(unsigned sz);

		/// Move a component at index 'from' to a new location at index 'to'
		/// The memory at index 'to' shoudn't be used by another component before the move;
		void move_instance(unsigned from, unsigned to);
	};
}

#endif // MESH_RENDER_SYSTEM_H_HEADER_GUARD