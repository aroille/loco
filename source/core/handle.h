#ifndef HANDLE_H_HEADER_GUARD
#define HANDLE_H_HEADER_GUARD

#include <stdint.h>
#include <deque>
#include <vector>

namespace loco
{
	template <typename T, size_t index_bits, size_t generation_bits>
	struct GenericHandle
	{
		T id;
		
		inline T index() const { return id & _index_mask; } 
		inline T generation() const { return (id >> _index_bits) & _generation_mask; }

		static const T _index_bits = index_bits;
		static const T _index_mask = (1 << index_bits) - 1;
		static const T _generation_bits = generation_bits;
		static const T _generation_mask = (1 << generation_bits) - 1;
	};

	template <typename T, typename U, typename V, size_t MIN_FREE_INDICES = 1024>
	class GenericHandleManager
	{
	public:
		GenericHandleManager();
		T create();
		void destroy(T h);
		bool is_alive(T h) const;

	private:
		T make_handle(U index, V generation) const;

		std::deque<U>	_free_indices;
		std::vector<V>	_generation;
	};

	//==========================================================================
	template <typename T, typename U, typename V, size_t MIN_FREE_INDICES>
	GenericHandleManager<T, U, V, MIN_FREE_INDICES>::GenericHandleManager()
	{
		LOCO_ASSERTF(((sizeof(U)* 8) >= T::_index_bits) && ((sizeof(V)* 8) >= T::_generation_bits),
			"HandleManager",
			"Handle type not compatible with HandleManager type");
	};

	//==========================================================================
	template <typename T, typename U, typename V, size_t MIN_FREE_INDICES>
	T GenericHandleManager<T, U, V, MIN_FREE_INDICES>::create()
	{
		unsigned idx;
		if (_free_indices.size() > MIN_FREE_INDICES)
		{
			idx = _free_indices.front();
			_free_indices.pop_front();
		}
		else
		{
			_generation.push_back(0);
			idx = _generation.size() - 1;
			LOCO_ASSERTF(idx < (1 << T::_index_bits), "HandleManager", "Handle cache overflow. %d (max: %d)", idx + 1, (1 << T::_index_bits));
		}
		return make_handle(idx, _generation[idx]);
	};

	//==========================================================================
	template <typename T, typename U, typename V, size_t MIN_FREE_INDICES>
	void GenericHandleManager<T, U, V, MIN_FREE_INDICES>::destroy(T handle)
	{
		const unsigned idx = handle.index();
		++_generation[idx];
		_free_indices.push_back(idx);
	};

	//==========================================================================
	template <typename T, typename U, typename V, size_t MIN_FREE_INDICES>
	bool GenericHandleManager<T, U, V, MIN_FREE_INDICES>::is_alive(T handle) const
	{
		return _generation[handle.index()] == handle.generation();
	};

	//==========================================================================
	template <typename T, typename U, typename V, size_t MIN_FREE_INDICES>
	T GenericHandleManager<T, U, V, MIN_FREE_INDICES>::make_handle(U index, V generation) const
	{
		return T{ (index & T::_index_mask) | ((generation << T::_index_bits) & T::_generation_mask) };
	};

	//==========================================================================
	typedef GenericHandle<uint32_t, 24, 8> HandleI24G8;
	typedef GenericHandleManager<HandleI24G8, uint32_t, uint8_t> HandleManagerI24G8;

	//==========================================================================
	typedef GenericHandle<uint16_t, 8, 8> HandleI8G8;
	typedef GenericHandleManager<HandleI8G8, uint8_t, uint8_t> HandleManagerI8G8;
}

#endif // HANDLE_H_HEADER_GUARD