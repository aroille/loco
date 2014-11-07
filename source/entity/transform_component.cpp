#include "transform_component.h"
#include "debug.h"

namespace loco
{
	const TransformComponent::Instance TransformComponent::Instance::invalid = { 0xffffffff };


	TransformComponent::TransformComponent()
	{
		_data = InstanceData{};
		allocate(2);	
	}

	TransformComponent::~TransformComponent()
	{
		free(_data.buffer);
	}

	TransformComponent::Instance TransformComponent::create(Entity e)
	{
		LOCO_ASSERTF(!is_valid(lookup(e)), "An entity can't have several transform components in the same world");

		if ((_data.size + 1) >= _data.capacity)
			allocate(_data.capacity * 2);

		Instance i = make_instance(_data.size);
		_data.entity[i.i] = e;
		_data.local[i.i] = Matrix4x4::identity;
		_data.world[i.i] = Matrix4x4::identity;
		_data.parent[i.i] = Instance::invalid;
		_data.first_child[i.i] = Instance::invalid;
		_data.next_sibling[i.i] = Instance::invalid;
		_data.prev_sibling[i.i] = Instance::invalid;

		_map[e.id] = i.i;
		++_data.size;

		return i;
	}

	void TransformComponent::destroy(Entity e)
	{
		Instance i = lookup(e);
		LOCO_ASSERTF(is_valid(i), "Entity not found (id:%s)", e.id);

		unlink(i);

		// update children
		Instance child = _data.first_child[i.i];
		while (is_valid(child))
		{
			_data.parent[child.i] = Instance::invalid;
			transform(Matrix4x4::identity, child);
			child = _data.next_sibling[child.i];
		}

		_map.erase(_data.entity[i.i].id);

		// move the instance at [size-1] to the initial index of the destroyed instance
		if (_data.size > 1)
		{         
			move_instance(_data.size - 1, i.i);
			_map[_data.entity[i.i].id] = i.i;
		}

		--_data.size;
	}

	void TransformComponent::link(Instance child, Instance parent)
	{
		LOCO_ASSERTF(is_valid(child), "Child transform componennt invalid");
		LOCO_ASSERTF(is_valid(parent), "Parent transform component invalid");

		// if child has already a parent : unlink
		if (is_valid(_data.parent[child.i]))
			unlink(child);

		// if parent has already childrens
		// add the new children at the beginning of the linked list
		Instance parent_first_child = _data.first_child[parent.i];
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


	void TransformComponent::unlink(Instance child)
	{
		LOCO_ASSERTF(is_valid(child), "Child transform component invalid");

		Instance parent = _data.parent[child.i];
		if (!is_valid(parent))
			return;

		// update the "first_child" value of the parent
		if (_data.first_child[parent.i] == child)
		{
			if (is_valid(_data.next_sibling[child.i]))
				_data.first_child[parent.i] = _data.next_sibling[child.i];
			else
				_data.first_child[parent.i] = Instance::invalid;
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

	TransformComponent::Instance TransformComponent::lookup(Entity e)
	{
		auto it = _map.find(e.id);
		return (it == _map.end()) ? Instance::invalid : make_instance(it->second);
	}

	bool TransformComponent::is_valid(Instance i)
	{
		return (i.i != Instance::invalid.i);
	}

	Matrix4x4 TransformComponent::local(Instance i)
	{
		return _data.local[i.i];
	}

	void TransformComponent::set_local(Instance i, const Matrix4x4& m)
	{
		_data.local[i.i] = m;
		Instance parent = _data.parent[i.i];
		Matrix4x4 parent_tm = is_valid(parent) ? _data.world[parent.i] : Matrix4x4::identity;
		transform(parent_tm, i);
	}

	Matrix4x4 TransformComponent::world(Instance i)
	{
		return _data.world[i.i];
	}

	void TransformComponent::allocate(unsigned sz)
	{
		LOCO_ASSERT(sz > _data.size);

		InstanceData new_data;
		const unsigned bytes = sz * (sizeof(Entity) + 2 * sizeof(Matrix4x4) + 4 * sizeof(Instance));
		new_data.buffer = malloc(bytes);
		new_data.size= _data.size;
		new_data.capacity = sz;

		new_data.entity = (Entity *)(new_data.buffer);
		new_data.local = (Matrix4x4 *)(new_data.entity + sz);
		new_data.world = new_data.local + sz;
		new_data.parent = (Instance *)(new_data.world + sz);
		new_data.first_child = new_data.parent + sz;
		new_data.next_sibling = new_data.first_child + sz;
		new_data.prev_sibling = new_data.next_sibling + sz;

		memcpy(new_data.entity, _data.entity, _data.size * sizeof(Entity));
		memcpy(new_data.local, _data.local, _data.size * sizeof(Matrix4x4));
		memcpy(new_data.world, _data.world, _data.size * sizeof(Matrix4x4));
		memcpy(new_data.parent, _data.parent, _data.size * sizeof(Instance));
		memcpy(new_data.first_child, _data.first_child, _data.size * sizeof(Instance));
		memcpy(new_data.next_sibling, _data.next_sibling, _data.size * sizeof(Instance));
		memcpy(new_data.prev_sibling, _data.prev_sibling, _data.size * sizeof(Instance));

		free(_data.buffer);
		_data = new_data;
	}

	TransformComponent::Instance TransformComponent::make_instance(unsigned i)
	{
		Instance inst = { i };
		return inst;
	}

	void TransformComponent::transform(const Matrix4x4& parent, Instance i)
	{
		_data.world[i.i] = parent * _data.local[i.i];

		Instance child = _data.first_child[i.i];
		while (is_valid(child))
		{
			transform(_data.world[i.i], child);
			child = _data.next_sibling[child.i];
		}
	}

	void TransformComponent::move_instance(unsigned from, unsigned to)
	{
		Instance new_instance = make_instance(to);
		
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
			_data.first_child[_data.parent[to].i] = new_instance;
		}

		// update the "parent" value of the children
		Instance child = _data.first_child[to];
		while (is_valid(child))
		{
			_data.parent[child.i] = new_instance;
			child = _data.next_sibling[child.i];
		}

		// update the siblings
		if (is_valid(_data.next_sibling[to]))
			_data.prev_sibling[_data.next_sibling[to].i] = new_instance;
		if (is_valid(_data.prev_sibling[to]))
			_data.next_sibling[_data.prev_sibling[to].i] = new_instance;
	}
}
