#ifndef TRANSFORM_COMPONENT_H_HEADER_GUARD
#define TRANSFORM_COMPONENT_H_HEADER_GUARD

#include "entity.h"
#include "math_types.h"

#include <unordered_map>

namespace loco
{
	/// Transform component (just a handle)
	struct TransformComponent
	{
		unsigned i;
		//---------
		bool operator==(TransformComponent const& in) const { return i == in.i; };
		static const TransformComponent invalid;
	};

	/// Manager for transform components
	/// Once data capacity reached, capacity will automatically doubled
	class TransformSystem
	{
		public:


			TransformSystem();
			~TransformSystem();

			/// Attach a new transform component to entity e
			TransformComponent create(Entity e);

			/// Get transform component attached to entity e
			TransformComponent lookup(Entity e);

			/// Free the transform component attached to entity e
			void destroy(Entity e);

			/// Check if component c is different from TransformComponent::invalid
			bool is_valid(TransformComponent c);

			/// Create child/parent relationship between two transform component
			void link(TransformComponent child, TransformComponent parent);

			/// Detach child from its parent 
			void unlink(TransformComponent child);

			/// Get local transform matrix
			Matrix4x4 local_matrix(TransformComponent c);

			/// Set local transform matrix (it also updates children instances)
			void set_local_matrix(TransformComponent c, const Matrix4x4& m);

			/// Get world transform matrix 
			Matrix4x4 world_matrix(TransformComponent c);

			/// Get parent
			TransformComponent parent(TransformComponent c);

			/// Get first child
			TransformComponent first_child(TransformComponent c);

			/// Get next sibling
			TransformComponent next_sibling(TransformComponent c);

			/// Get previous sibling
			TransformComponent prev_sibling(TransformComponent c);

		private:

			struct ComponentData
			{
				unsigned size;				///< Number of used instance
				unsigned capacity;			///< Number of allocated instance in arrays
				char* buffer;				///< Buffer with instance data

				Matrix4x4* local;			///< Local transform relative to parent
				Matrix4x4* world;			///< World transform
				Entity* entity;				///< The entity owning this instance
				TransformComponent* parent;			///< Parent instance of this instance
				TransformComponent* first_child;		///< First child of this instance
				TransformComponent* next_sibling;		///< The next sibling of this instance
				TransformComponent* prev_sibling;		///< The next sibling of this instance
			};
			ComponentData _data;

			/// Map an Entity (key) with a TransformComponent (value)
			std::unordered_map<unsigned, unsigned> _map;

			/// Increase the capicity of the data buffer to an component count of sz
			void allocate(unsigned sz);

			/// Return an component from an index
			TransformComponent make_component(unsigned i);

			/// Update the world matrix of component c and of its children
			void transform(const Matrix4x4& parent, TransformComponent c);

			/// Move a component at index 'from' to a new location at index 'to'
			/// This function will also update all transform component with reference to this component
			/// The memory at index 'to' shoudn't be used by another component before the move;
			void move_instance(unsigned from, unsigned to);
	};
}

#endif //TRANSFORM_COMPONENT_H_HEADER_GUARD

