#ifndef TRANSFORM_COMPONENT_H_HEADER_GUARD
#define TRANSFORM_COMPONENT_H_HEADER_GUARD

#include "entity.h"
#include "math_types.h"

#include <unordered_map>

namespace loco
{
	class TransformComponent
	{
		public:

			struct Instance 
			{
				unsigned i;

				inline bool operator==(Instance const& in) const { return i == in.i; };

				static const Instance invalid;
			};

			TransformComponent();
			~TransformComponent();

			Instance create(Entity e);
			Instance lookup(Entity e);
			void destroy(Entity e);

			/// Create a child/parent relationship between two entities
			void link(Instance child, Instance parent);

			/// Detach the child from its parent 
			void unlink(Instance child);

			bool is_valid(Instance e);

			Matrix4x4 local(Instance i);
			void set_local(Instance i, const Matrix4x4& m);

			Matrix4x4 world(Instance i);


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

			void allocate(unsigned sz);
			Instance make_instance(unsigned i);
			void transform(const Matrix4x4& parent, Instance i);
			void move_instance(unsigned from, unsigned to);

			InstanceData _data;

			/// Map an Entity id (key) with a Instance id (value)
			std::unordered_map<unsigned, unsigned> _map;
	};
}

#endif //TRANSFORM_COMPONENT_H_HEADER_GUARD