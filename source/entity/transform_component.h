#ifndef TRANSFORM_COMPONENT_H_HEADER_GUARD
#define TRANSFORM_COMPONENT_H_HEADER_GUARD

#include "entity.h"
#include "math_types.h"

#include <unordered_map>

namespace loco
{
	/// Manager for transform components
	/// Once data capacity reached, capacity will automatically doubled
	class TransformComponent
	{
		public:

			/// Instance handle
			struct Instance 
			{
				unsigned i;
				//---------
				static const Instance invalid;
				bool operator==(Instance const& in) const { return i == in.i; };
			};

			TransformComponent();
			~TransformComponent();

			/// Attach a new transform component to entity e
			Instance create(Entity e);

			/// Get transform component attached to entity e
			Instance lookup(Entity e);

			/// Free the transform component attached to entity e
			void destroy(Entity e);

			/// Check if instance e is different from Instance::invalid
			bool is_valid(Instance e);

			/// Create child/parent relationship between two transform component instances
			void link(Instance child, Instance parent);

			/// Detach child from its parent 
			void unlink(Instance child);

			/// Get local transform matrix of instance i
			Matrix4x4 local_matrix(Instance i);

			/// Set local transform matrix of instance i (it also updates children instances)
			void set_local_matrix(Instance i, const Matrix4x4& m);

			/// Get world transform matrix of instance i
			Matrix4x4 world_matrix(Instance i);

			/// Get parent of instance i
			Instance parent(Instance i);

			/// Get first child of instance i
			Instance first_child(Instance i);

			/// Get next sibling of instance i
			Instance next_sibling(Instance i);

			/// Get previous sibling of instance i
			Instance prev_sibling(Instance i);



		private:

			struct InstanceData
			{
				unsigned size;				///< Number of used instance
				unsigned capacity;			///< Number of allocated instance in arrays
				void* buffer;				///< Buffer with instance data

				Entity* entity;				///< The entity owning this instance
				Matrix4x4* local;			///< Local transform relative to parent
				Matrix4x4* world;			///< World transform
				Instance* parent;			///< Parent instance of this instance
				Instance* first_child;		///< First child of this instance
				Instance* next_sibling;		///< The next sibling of this instance
				Instance* prev_sibling;		///< The next sibling of this instance
			};
			InstanceData _data;

			/// Map an Entity id (key) with a Instance id (value)
			std::unordered_map<unsigned, unsigned> _map;

			/// Increase the capicity of the data buffer to an Instance count of sz
			void allocate(unsigned sz);

			/// Return an instance from an index
			Instance make_instance(unsigned i);

			/// Update the world matrix of Instance i and of its children
			void transform(const Matrix4x4& parent, Instance i);

			/// Move an instance at index 'from' to a new location at index 'to'
			/// This function will also update all instances with reference to this instance
			/// The memory at index 'to' shoudn't be used by another instance before the move;
			void move_instance(unsigned from, unsigned to);
	};
}

#endif //TRANSFORM_COMPONENT_H_HEADER_GUARD