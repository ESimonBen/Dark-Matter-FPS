#pragma once
#include <cassert>

#ifdef CORE_ENABLE_ASSERTS
	#define CORE_ASSERT(x)\
		if (!(x)) {                                                            \
        std::cerr << "Assertion failed: " << #x << "\n"                    \
                  << "File: " << __FILE__ << "\n"                          \
                  << "Line: " << __LINE__ << std::endl;                    \
        std::abort();                                                      \
        }
#else
	#define CORE_ASSERT(x) (void(0))
#endif