#pragma once
#ifndef _HEADERS_ASSERT_H_
#define _HEADERS_ASSERT_H_

#define ASSERION_ENABLE 1

#if defined(_DEBUG) and ASSERION_ENABLE and defined(_MSC_VER) 


//make break into the code
#define debugBreak() __debugbreak()

//if expr is false then we sturt debugging.
#define ASSERT(expr) \
{                    \
	if (expr) {}     \
	else             \
	{   \
		std::cout << __FILE__ << " " << __LINE__ << " " << #expr << std::endl;			\
		debugBreak();\
	}                \
}

#else

//this macro is empty if assertion off.
#define ASSERT(expr)


#endif // ASSERION_ENABLE

#endif // _HEADERS_ASSERT_H_