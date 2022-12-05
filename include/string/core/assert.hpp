#include <cassert>

#if STRING_RELEASE == 0
#define STRING_STATIC_ASSERT(expression, message) static_assert(expression, message)
#define STRING_ASSERT(...) assert(__VA_ARGS__)
#endif

