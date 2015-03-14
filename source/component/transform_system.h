#ifndef TRANSFORM_SYSTEM_H_HEADER_GUARD
#define TRANSFORM_SYSTEM_H_HEADER_GUARD

#include "entity.h"
#include "handle.h"
#include "math_types.h"
#include <unordered_map>

namespace loco
{
	using namespace math;

	struct TransformComponent
	{
		HandleI24G8 handle;
		static const TransformComponent null;
	};

	/// Manager for transform components
	/// (One transformSystem per world)
	///
	/// @remark
	/// Once data capacity reached, capacity will automatically doubled
	///
	class TransformSystem
	{
		public:

			TransformSystem();
			~TransformSystem();

			/// Attach a new transform component to entity e
			TransformComponent create(Entity e);

			/// Get transform component attached to entity e
			TransformComponent lookup(Entity e) const;

			/// Check if component c is valid
			bool is_valid(TransformComponent c) const;

			/// Free the transform component attached to entity e
			void destroy(Entity e);

			/// Create child/parent relationship between two transform component
			void link(TransformComponent child, TransformComponent parent);

			/// Detach child from its parent 
			void unlink(TransformComponent child);

			/// Get local transform matrix
			Matrix4x4 local_matrix(TransformComponent c) const;

			/// Set local transform matrix (it also updates children instances)
			void set_local_matrix(TransformComponent c, const Matrix4x4& m);

			/// Get world transform matrix 
			Matrix4x4 world_matrix(TransformComponent c) const;

			/// Get parent
			TransformComponent parent(TransformComponent c) const;

			/// Get first child
			TransformComponent first_child(TransformComponent c) const;

			/// Get next sibling
			TransformComponent next_sibling(TransformComponent c) const;

			/// Get previous sibling
			TransformComponent prev_sibling(TransformComponent c) const;

			/// Garbage collector
			void gc(const EntityManager& em);

			struct DataIndex
			{
				unsigned i;
				//---------
				bool operator==(DataIndex const& in) const { return i == in.i; };
				static const DataIndex invalid;
			};

			struct ComponentData
			{
				unsigned size;				///< Number of used instance
				unsigned capacity;		///< Number of allocated instance in arrays
				unsigned dirty;				///< Count of updated transform which need to be synchronized
				char* buffer;					///< Buffer with instance data
					
				Matrix4x4* local;				///< Local transform relative to parent
				Matrix4x4* world;				///< World transform
				Entity* entity;					///< The entity owning this instance
				TransformComponent* component;			///< The component owning this instance data
				DataIndex* parent;				///< Parent instance of this instance
				DataIndex* first_child;			///< First child of this instance
				DataIndex* next_sibling;		///< The next sibling of this instance
				DataIndex* prev_sibling;		///< The prev sibling of this instance

				DataIndex* lut;					///< Look at table : Component / ComponentData index
			};
			ComponentData _data;

	private:

			/// Handle manager (Component = handle)
			HandleManagerI24G8	_handle_mgr;

			/// Map an Entity (key) with a Component (value)
			std::unordered_map<unsigned, TransformComponent> _map;

			/// Detach child from its parent 
			void unlink(DataIndex child);

			/// Check if component c is valid
			bool is_valid(DataIndex c) const;

			/// Increase the capicity of the data buffer to an component count of sz
			void allocate(unsigned sz);

			/// Return a _data index from a component
			inline DataIndex data_index(TransformComponent c) const
			{ 
				return _data.lut[c.handle.index()]; 
			};

			/// Update the world matrix of component at index i and of its children
			void transform(const Matrix4x4& parent, DataIndex i);

			/// Move a component at index 'from' to a new location at index 'to'
			/// This function will also update all transform component with reference to this component
			/// The memory at index 'to' shoudn't be used by another component before the move;
			void move_instance(unsigned from, unsigned to);

			void swap_instance(unsigned a, unsigned b);

			DataIndex set_updated(DataIndex i);
			
	};
}

#endif //TRANSFORM_SYSTEM_H_HEADER_GUARD

