#ifndef READER_WRITER_H_HEADER_GUARD
#define READER_WRITER_H_HEADER_GUARD

#include "defines.h"

#include <string.h>
#include <stdint.h>

namespace loco
{
	// utils

	inline uint32_t uint32_min(uint32_t _a, uint32_t _b)
	{
		return _a > _b ? _b : _a;
	}

	inline uint32_t uint32_max(uint32_t _a, uint32_t _b)
	{
		return _a > _b ? _a : _b;
	}

	inline uint32_t uint32_clamp(uint32_t _a, uint32_t _min, uint32_t _max)
	{
		const uint32_t tmp = uint32_max(_a, _min);
		const uint32_t result = uint32_min(tmp, _max);

		return result;
	}

	inline int64_t int64_min(int64_t _a, int64_t _b)
	{
		return _a < _b ? _a : _b;
	}

	inline int64_t int64_max(int64_t _a, int64_t _b)
	{
		return _a > _b ? _a : _b;
	}

	inline int64_t int64_clamp(int64_t _a, int64_t _min, int64_t _max)
	{
		const int64_t min = int64_min(_a, _max);
		const int64_t result = int64_max(_min, min);

		return result;
	}


	struct Whence
	{
		enum Enum
		{
			Begin,
			Current,
			End,
		};
	};

	class MemoryReader
	{
	public:
		MemoryReader(const void* _data, uint32_t _size)
			: m_data((const uint8_t*)_data)
			, m_pos(0)
			, m_top(_size)
		{
		}

		~MemoryReader()
		{
		}

		int64_t seek(int64_t _offset = 0, Whence::Enum _whence = Whence::Current)
		{
			switch (_whence)
			{
			case Whence::Begin:
				m_pos = _offset;
				break;

			case Whence::Current:
				m_pos = int64_clamp(m_pos + _offset, 0, m_top);
				break;

			case Whence::End:
				m_pos = int64_clamp(m_top - _offset, 0, m_top);
				break;
			}

			return m_pos;
		}

		int32_t read(void* _data, int32_t _size)
		{
			int64_t reminder = m_top - m_pos;
			int32_t size = uint32_min(_size, int32_t(reminder > INT32_MAX ? INT32_MAX : reminder));
			memcpy(_data, &m_data[m_pos], size);
			m_pos += size;
			return size;
		}

		template<typename T>
		int32_t read(T& value)
		{
			return read(&value, sizeof(T));
		}

		inline int64_t skip(int64_t _offset)
		{
			return seek(_offset, Whence::Current);
		}

		const uint8_t* getDataPtr() const
		{
			return &m_data[m_pos];
		}

		int64_t getPos() const
		{
			return m_pos;
		}

		int64_t remaining() const
		{
			return m_top - m_pos;
		}

	private:
		const uint8_t* m_data;
		int64_t m_pos;
		int64_t m_top;
	};




}

#endif // READER_WRITER_H_HEADER_GUARD

