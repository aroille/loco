#ifndef TRANSFORM_COMPONENT_H_HEADER_GUARD
#define TRANSFORM_COMPONENT_H_HEADER_GUARD

#include "entity.h"
#include "math_types.h"

#include <unordered_map>

namespace loco
{

	/// Manager for transform components
	/// Once data capacity reached, capacity will automatically doubled
	class TransformSystem
	{
		public:

			/// Transform component (just a handle)
			struct Component
			{
				unsigned i;
				//---------
				bool operator==(Component const& in) const { return i == in.i; };
				static const Component invalid;
			};

			TransformSystem();
			~TransformSystem();

			/// Attach a new transform component to entity e
			Component create(Entity e);

			/// Get transform component attached to entity e
			Component lookup(Entity e);

			/// Free the transform component attached to entity e
			void destroy(Entity e);

			/// Check if component c is different from TransformComponent::invalid
			bool is_valid(Component c);

			/// Create child/parent relationship between two transform component
			void link(Component child, Component parent);

			/// Detach child from its parent 
			void unlink(Component child);

			/// Get local transform matrix
			Matrix4x4 local_matrix(Component c);

			/// Set local transform matrix (it also updates children instances)
			void set_local_matrix(Component c, const Matrix4x4& m);

			/// Get world transform matrix 
			Matrix4x4 world_matrix(Component c);

			/// Get parent
			Component parent(Component c);

			/// Get first child
			Component first_child(Component c);

			/// Get next sibling
			Component next_sibling(Component c);

			/// Get previous sibling
			Component prev_sibling(Component c);

		private:

			struct ComponentData
			{
				unsigned size;				///< Number of used instance
				unsigned capacity;			///< Number of allocated instance in arrays
				char* buffer;				///< Buffer with instance data

				Matrix4x4* local;			///< Local transform relative to parent
				Matrix4x4* world;			///< World transform
				Entity* entity;				///< The entity owning this instance
				Component* parent;			///< Parent instance of this instance
				Component* first_child;		///< First child of this instance
				Component* next_sibling;		///< The next sibling of this instance
				Component* prev_sibling;		///< The next sibling of this instance
			};
			ComponentData _data;

			/// Map an Entity (key) with a TransformComponent (value)
			std::unordered_map<unsigned, unsigned> _map;

			/// Increase the capicity of the data buffer to an component count of sz
			void allocate(unsigned sz);

			/// Return an component from an index
			Component make_component(unsigned i);

			/// Update the world matrix of component c and of its children
			void transform(const Matrix4x4& parent, Component c);

			/// Move a component at index 'from' to a new location at index 'to'
			/// This function will also update all transform component with reference to this component
			/// The memory at index 'to' shoudn't be used by another component before the move;
			void move_instance(unsigned from, unsigned to);
	};
}

#endif //TRANSFORM_COMPONENT_H_HEADER_GUARD

