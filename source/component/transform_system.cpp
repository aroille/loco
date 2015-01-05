#include "transform_system.h"
#include "debug.h"

namespace loco
{
	const TransformSystem::DataIndex TransformSystem::DataIndex::invalid = { 0xffffffff };


	TransformSystem::TransformSystem()
	{
		_data = ComponentData{};
		allocate(256);	
	}

	TransformSystem::~TransformSystem()
	{
		free(_data.buffer);
	}

	TransformSystem::Component TransformSystem::create(Entity e)
	{
		LOCO_ASSERTF(!is_valid(lookup(e)), LOCO_LOG_TRANSFORM_SYSTEM, "An entity can't have several transform components in the same world");

		Component c = _handle_mgr.create();

		if ((c.index() + 1) >= _data.capacity)
			allocate(_data.capacity * 2);
		
		DataIndex i = { _data.size };

		_data.local[i.i] = Matrix4x4::identity;
		_data.world[i.i] = Matrix4x4::identity;
		_data.entity[i.i] = e;
		_data.component[i.i] = c;
		_data.parent[i.i] = DataIndex::invalid;
		_data.first_child[i.i] = DataIndex::invalid;
		_data.next_sibling[i.i] = DataIndex::invalid;
		_data.prev_sibling[i.i] = DataIndex::invalid;

		_data.lut[c.index()] = i;
		_map[e.id] = c;

		++_data.size;

		return c;
	}

	void TransformSystem::destroy(Entity e)
	{
		Component c = lookup(e);
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "TransformComponent not found for entity (id:%s)", e.id);
		_map.erase(e.id);

		DataIndex i = data_index(c);
		
		// unlink the component and its children
		DataIndex child = _data.first_child[i.i];
		while (is_valid(child))
		{
			unlink(child);
			child = _data.next_sibling[child.i];
		}

		unlink(i);

		// move the instance at [size-1] to the initial index of the destroyed instance
		if (_data.size > 1)
		{         
			move_instance(_data.size - 1, i.i);
			_map[_data.entity[i.i].id] = _data.component[i.i];
		}

		--_data.size;
	}

	TransformSystem::Component TransformSystem::lookup(Entity e)
	{
		auto it = _map.find(e.id);
		return (it == _map.end()) ? TransformSystem::Component{ -1 } : it->second;
	}

	void TransformSystem::link(Component child, Component parent)
	{
		LOCO_ASSERTF(is_valid(child), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		LOCO_ASSERTF(is_valid(parent), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");

		DataIndex child_i = data_index(child);
		DataIndex parent_i = data_index(parent);

		// if child has already a parent : unlink
		if (is_valid(_data.parent[child_i.i]))
			unlink(child_i);

		// if parent has already childrens
		// add the new children at the beginning of the linked list
		DataIndex parent_first_child = _data.first_child[parent_i.i];
		if (is_valid(parent_first_child))
		{
			_data.prev_sibling[parent_first_child.i] = child_i;
			_data.next_sibling[child_i.i] = parent_first_child;
		}

		_data.first_child[parent_i.i] = child_i;
		_data.parent[child_i.i] = parent_i;
		_data.local[child_i.i] = Matrix4x4::identity;
		transform(_data.world[parent_i.i], child_i);
	}

	void TransformSystem::unlink(Component child)
	{
		LOCO_ASSERTF(is_valid(child), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(child);
		unlink(i);
	}

	void TransformSystem::unlink(DataIndex child)
	{
		DataIndex parent = _data.parent[child.i];
		if (!is_valid(parent))
			return;

		// update the "first_child" value of the parent
		if (_data.first_child[parent.i] == child)
		{
			if (is_valid(_data.next_sibling[child.i]))
				_data.first_child[parent.i] = _data.next_sibling[child.i];
			else
				_data.first_child[parent.i] = DataIndex::invalid;
		}

		// update siblings 
		if (is_valid(_data.next_sibling[child.i]))
			_data.prev_sibling[_data.next_sibling[child.i].i] = _data.prev_sibling[child.i];
		if (is_valid(_data.prev_sibling[child.i]))
			_data.next_sibling[_data.prev_sibling[child.i].i] = _data.next_sibling[child.i];

		// reset parent
		_data.parent[child.i] = DataIndex::invalid;

		// local = world
		_data.local[child.i] = _data.world[child.i];

		// update world matrix for the instance and its children
		transform(Matrix4x4::identity, child);
	}

	bool TransformSystem::is_valid(Component c)
	{
		return _handle_mgr.is_alive(c);
	}

	bool TransformSystem::is_valid(DataIndex i)
	{
		return (i.i != DataIndex::invalid.i);
	}

	Matrix4x4 TransformSystem::local_matrix(Component c)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(c);
		return _data.local[i.i];
	}

	void TransformSystem::set_local_matrix(Component c, const Matrix4x4& m)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(c);
		_data.local[i.i] = m;
		DataIndex parent = _data.parent[i.i];
		Matrix4x4 parent_tm = is_valid(parent) ? _data.world[parent.i] : Matrix4x4::identity;
		transform(parent_tm, i);
	}

	Matrix4x4 TransformSystem::world_matrix(Component c)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(c);
		return _data.world[i.i];
	}

	TransformSystem::Component TransformSystem::parent(Component c)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(c);
		return _data.component[_data.parent[i.i].i];
	}

	TransformSystem::Component TransformSystem::first_child(Component c)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(c);
		return _data.component[_data.first_child[i.i].i];
	}

	TransformSystem::Component TransformSystem::next_sibling(Component c)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(c);
		return _data.component[_data.next_sibling[i.i].i];
	}

	TransformSystem::Component TransformSystem::prev_sibling(Component c)
	{
		LOCO_ASSERTF(is_valid(c), LOCO_LOG_TRANSFORM_SYSTEM, "Transform component not valid");
		DataIndex i = data_index(c);
		return _data.component[_data.prev_sibling[i.i].i];
	}

	void TransformSystem::allocate(unsigned sz)
	{
		LOCO_ASSERT(sz > _data.size);

		ComponentData new_data;
		unsigned alignement = 16; // Matrix4x4 need to be aligned on 16 octets (Matrix4x4* are first members of the struct)
		const unsigned bytes = sz * (sizeof(Entity)+sizeof(Component)+2 * sizeof(Matrix4x4)+5 * sizeof(DataIndex)+alignement);
		new_data.buffer = (char*)malloc(bytes);
		new_data.size= _data.size;
		new_data.capacity = sz;

		unsigned offset = alignement - ((unsigned)new_data.buffer & (alignement - 1));

		new_data.local = (Matrix4x4 *)(new_data.buffer + offset);
		new_data.world = new_data.local + sz;
		new_data.entity = (Entity*)(new_data.world + sz);
		new_data.component = (Component*)(new_data.entity + sz);
		new_data.parent = (DataIndex *)(new_data.component + sz);
		new_data.first_child = new_data.parent + sz;
		new_data.next_sibling = new_data.first_child + sz;
		new_data.prev_sibling = new_data.next_sibling + sz;
		new_data.lut = new_data.prev_sibling + sz;
		
		memcpy(new_data.local, _data.local, _data.size * sizeof(Matrix4x4));
		memcpy(new_data.world, _data.world, _data.size * sizeof(Matrix4x4));
		memcpy(new_data.entity, _data.entity, _data.size * sizeof(Entity));
		memcpy(new_data.component, _data.component, _data.size * sizeof(Component));
		memcpy(new_data.parent, _data.parent, _data.size * sizeof(DataIndex));
		memcpy(new_data.first_child, _data.first_child, _data.size * sizeof(DataIndex));
		memcpy(new_data.next_sibling, _data.next_sibling, _data.size * sizeof(DataIndex));
		memcpy(new_data.prev_sibling, _data.prev_sibling, _data.size * sizeof(DataIndex));
		memcpy(new_data.lut, _data.lut, _data.size * sizeof(DataIndex));

		free(_data.buffer);
		_data = new_data;
	}

	void TransformSystem::transform(const Matrix4x4& parent, DataIndex i)
	{
		_data.world[i.i] = parent * _data.local[i.i];

		DataIndex child = _data.first_child[i.i];
		while (is_valid(child))
		{
			transform(_data.world[i.i], child);
			child = _data.next_sibling[child.i];
		}
	}

	void TransformSystem::move_instance(unsigned from, unsigned to)
	{
		Component c = _data.component[from];
		DataIndex new_data_index = DataIndex{ to };

		_data.entity[to] = _data.entity[from];
		_data.local[to] = _data.local[from];
		_data.world[to] = _data.world[from];
		_data.parent[to] = _data.parent[from];
		_data.first_child[to] = _data.first_child[from];
		_data.next_sibling[to] = _data.next_sibling[from];
		_data.prev_sibling[to] = _data.prev_sibling[from];

		// update the "first_child" value of the parent
		if (is_valid(_data.parent[to]) && (_data.first_child[_data.parent[to].i].i == from))
		{
			_data.first_child[_data.parent[to].i] = new_data_index;
		}

		// update the "parent" value of the children
		DataIndex child = _data.first_child[to];
		while (is_valid(child))
		{
			_data.parent[child.i] = new_data_index;
			child = _data.next_sibling[child.i];
		}

		// update the siblings
		if (is_valid(_data.next_sibling[to]))
			_data.prev_sibling[_data.next_sibling[to].i] = new_data_index;
		if (is_valid(_data.prev_sibling[to]))
			_data.next_sibling[_data.prev_sibling[to].i] = new_data_index;

		// update lut "component / component data index"
		_data.lut[c.index()] = new_data_index;
	}

}

