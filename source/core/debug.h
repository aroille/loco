#ifndef DEBUG_H_HEADER_GUARD
#define DEBUG_H_HEADER_GUARD


#define LOCO_ASSERT(expr)				{ if(!(expr)) loco::assert_failed(#expr, __FILE__, __LINE__); }
#define LOCO_ASSERTF(expr, ...)		{ if(!(expr)) loco::assert_failed(#expr, __FILE__, __LINE__, ##__VA_ARGS__); }

namespace loco
{
	void assert_failed(const char* expr, const char* file, int line, const char* format = "", ...);
}

#endif // DEBUG_H_HEADER_GUARD

