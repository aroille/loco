#ifndef READER_WRITER_H_HEADER_GUARD
#define READER_WRITER_H_HEADER_GUARD

#include "defines.h"
#include "type.h"

#include <string.h>

namespace loco
{
	// utils

	inline uint32 uint32_min(uint32 _a, uint32 _b)
	{
		return _a > _b ? _b : _a;
	}

	inline uint32 uint32_max(uint32 _a, uint32 _b)
	{
		return _a > _b ? _a : _b;
	}

	inline uint32 uint32_clamp(uint32 _a, uint32 _min, uint32 _max)
	{
		const uint32 tmp = uint32_max(_a, _min);
		const uint32 result = uint32_min(tmp, _max);

		return result;
	}

	inline int64 int64_min(int64 _a, int64 _b)
	{
		return _a < _b ? _a : _b;
	}

	inline int64 int64_max(int64 _a, int64 _b)
	{
		return _a > _b ? _a : _b;
	}

	inline int64 int64_clamp(int64 _a, int64 _min, int64 _max)
	{
		const int64 min = int64_min(_a, _max);
		const int64 result = int64_max(_min, min);

		return result;
	}


	enum class Whence
	{
		Begin,
		Current,
		End,
	};

	class MemoryReader
	{
	public:
		MemoryReader(const void* _data, uint32 _size)
			: m_data((const uint8*)_data)
			, m_pos(0)
			, m_top(_size)
		{
		}

		~MemoryReader()
		{
		}

		int64 seek(int64 _offset = 0, Whence _whence = Whence::Current)
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

		int32 read(void* _data, int32 _size)
		{
			int64 reminder = m_top - m_pos;
			int32 size = uint32_min(_size, int32(reminder > INT32_MAX ? INT32_MAX : reminder));
			memcpy(_data, &m_data[m_pos], size);
			m_pos += size;
			return size;
		}

		template<typename T>
		int32 read(T& value)
		{
			return read(&value, sizeof(T));
		}

		inline int64 skip(int64 _offset)
		{
			return seek(_offset, Whence::Current);
		}

		const uint8* getDataPtr() const
		{
			return &m_data[m_pos];
		}

		int64 getPos() const
		{
			return m_pos;
		}

		int64 remaining() const
		{
			return m_top - m_pos;
		}

	private:
		const uint8* m_data;
		int64 m_pos;
		int64 m_top;
	};




}

#endif // READER_WRITER_H_HEADER_GUARD

