#include "transform_component.h"
#include "debug.h"

namespace loco
{
	const TransformComponent TransformComponent::invalid = { 0xffffffff };


	TransformSystem::TransformSystem()
	{
		_data = ComponentData{};
		allocate(2);	
	}

	TransformSystem::~TransformSystem()
	{
		free(_data.buffer);
	}

	TransformComponent TransformSystem::create(Entity e)
	{
		LOCO_ASSERTF(!is_valid(lookup(e)), "An entity can't have several transform components in the same world");

		if ((_data.size + 1) >= _data.capacity)
			allocate(_data.capacity * 2);

		TransformComponent c = make_component(_data.size);
		_data.entity[c.i] = e;
		_data.local[c.i] = Matrix4x4::identity;
		_data.world[c.i] = Matrix4x4::identity;
		_data.parent[c.i] = TransformComponent::invalid;
		_data.first_child[c.i] = TransformComponent::invalid;
		_data.next_sibling[c.i] = TransformComponent::invalid;
		_data.prev_sibling[c.i] = TransformComponent::invalid;

		_map[e.id] = c.i;
		++_data.size;

		return c;
	}

	void TransformSystem::destroy(Entity e)
	{
		TransformComponent c = lookup(e);
		LOCO_ASSERTF(is_valid(c), "TransformSystem", "TransformComponent not found for entity (id:%s)", e.id);

		_map.erase(_data.entity[c.i].id);

		// unlink c and children of c
		unlink(c);

		TransformComponent child = _data.first_child[c.i];
		while (is_valid(child))
		{
			unlink(child);
			child = _data.next_sibling[child.i];
		}

		// move the instance at [size-1] to the initial index of the destroyed instance
		if (_data.size > 1)
		{         
			move_instance(_data.size - 1, c.i);
			_map[_data.entity[c.i].id] = c.i;
		}

		--_data.size;
	}

	void TransformSystem::link(TransformComponent child, TransformComponent parent)
	{
		LOCO_ASSERTF(is_valid(child), "Child transform componennt invalid");
		LOCO_ASSERTF(is_valid(parent), "Parent transform component invalid");

		// if child has already a parent : unlink
		if (is_valid(_data.parent[child.i]))
			unlink(child);

		// if parent has already childrens
		// add the new children at the beginning of the linked list
		TransformComponent parent_first_child = _data.first_child[parent.i];
		if (is_valid(parent_first_child))
		{
			_data.prev_sibling[parent_first_child.i] = child;
			_data.next_sibling[child.i] = parent_first_child;
		}

		_data.first_child[parent.i] = child;
		_data.parent[child.i] = parent;
		_data.local[child.i] = Matrix4x4::identity;
		transform(_data.world[parent.i], child);
	}


	void TransformSystem::unlink(TransformComponent child)
	{
		LOCO_ASSERTF(is_valid(child), "Child transform component invalid");

		TransformComponent parent = _data.parent[child.i];
		if (!is_valid(parent))
			return;

		// update the "first_child" value of the parent
		if (_data.first_child[parent.i] == child)
		{
			if (is_valid(_data.next_sibling[child.i]))
				_data.first_child[parent.i] = _data.next_sibling[child.i];
			else
				_data.first_child[parent.i] = TransformComponent::invalid;
		}

		// update siblings 
		if (is_valid(_data.next_sibling[child.i]))
			_data.prev_sibling[_data.next_sibling[child.i].i] = _data.prev_sibling[child.i];
		if (is_valid(_data.prev_sibling[child.i]))
			_data.next_sibling[_data.prev_sibling[child.i].i] = _data.next_sibling[child.i];

		// local = world
		_data.local[child.i] = _data.world[child.i];

		// update world matrix for the instance and its children
		transform(Matrix4x4::identity, child);
	}

	TransformComponent TransformSystem::lookup(Entity e)
	{
		auto it = _map.find(e.id);
		return (it == _map.end()) ? TransformComponent::invalid : make_component(it->second);
	}

	bool TransformSystem::is_valid(TransformComponent c)
	{
		return (c.i != TransformComponent::invalid.i);
	}

	Matrix4x4 TransformSystem::local_matrix(TransformComponent c)
	{
		return _data.local[c.i];
	}

	void TransformSystem::set_local_matrix(TransformComponent c, const Matrix4x4& m)
	{
		_data.local[c.i] = m;
		TransformComponent parent = _data.parent[c.i];
		Matrix4x4 parent_tm = is_valid(parent) ? _data.world[parent.i] : Matrix4x4::identity;
		transform(parent_tm, c);
	}

	Matrix4x4 TransformSystem::world_matrix(TransformComponent c)
	{
		return _data.world[c.i];
	}

	TransformComponent TransformSystem::parent(TransformComponent c)
	{
		return _data.parent[c.i];
	}

	TransformComponent TransformSystem::first_child(TransformComponent c)
	{
		return _data.first_child[c.i];
	}

	TransformComponent TransformSystem::next_sibling(TransformComponent c)
	{
		return _data.next_sibling[c.i];
	}

	TransformComponent TransformSystem::prev_sibling(TransformComponent c)
	{
		return _data.prev_sibling[c.i];
	}

	void TransformSystem::allocate(unsigned sz)
	{
		LOCO_ASSERT(sz > _data.size);

		ComponentData new_data;
		unsigned alignement = 16; // Matrix4x4 need to be aligned on 16 octets (Matrix4x4* are first members of the struct)
		const unsigned bytes = sz * (sizeof(Entity)+2 * sizeof(Matrix4x4)+4 * sizeof(TransformComponent) + alignement);
		new_data.buffer = (char*)malloc(bytes);
		new_data.size= _data.size;
		new_data.capacity = sz;

		unsigned offset = alignement - ((unsigned)new_data.buffer & (alignement - 1));

		new_data.local = (Matrix4x4 *)(new_data.buffer + offset);
		new_data.world = new_data.local + sz;
		new_data.entity = (Entity*)(new_data.world + sz);

		new_data.parent = (TransformComponent *)(new_data.world + sz);
		new_data.first_child = new_data.parent + sz;
		new_data.next_sibling = new_data.first_child + sz;
		new_data.prev_sibling = new_data.next_sibling + sz;

		memcpy(new_data.entity, _data.entity, _data.size * sizeof(Entity));
		memcpy(new_data.local, _data.local, _data.size * sizeof(Matrix4x4));
		memcpy(new_data.world, _data.world, _data.size * sizeof(Matrix4x4));
		memcpy(new_data.parent, _data.parent, _data.size * sizeof(TransformComponent));
		memcpy(new_data.first_child, _data.first_child, _data.size * sizeof(TransformComponent));
		memcpy(new_data.next_sibling, _data.next_sibling, _data.size * sizeof(TransformComponent));
		memcpy(new_data.prev_sibling, _data.prev_sibling, _data.size * sizeof(TransformComponent));

		free(_data.buffer);
		_data = new_data;
	}

	TransformComponent TransformSystem::make_component(unsigned i)
	{
		TransformComponent c = { i };
		return c;
	}

	void TransformSystem::transform(const Matrix4x4& parent, TransformComponent c)
	{
		_data.world[c.i] = parent * _data.local[c.i];

		TransformComponent child = _data.first_child[c.i];
		while (is_valid(child))
		{
			transform(_data.world[c.i], child);
			child = _data.next_sibling[child.i];
		}
	}

	void TransformSystem::move_instance(unsigned from, unsigned to)
	{
		TransformComponent new_component = make_component(to);
		
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
			_data.first_child[_data.parent[to].i] = new_component;
		}

		// update the "parent" value of the children
		TransformComponent child = _data.first_child[to];
		while (is_valid(child))
		{
			_data.parent[child.i] = new_component;
			child = _data.next_sibling[child.i];
		}

		// update the siblings
		if (is_valid(_data.next_sibling[to]))
			_data.prev_sibling[_data.next_sibling[to].i] = new_component;
		if (is_valid(_data.prev_sibling[to]))
			_data.next_sibling[_data.prev_sibling[to].i] = new_component;
	}

}

