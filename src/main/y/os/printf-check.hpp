//! \file
#ifndef Y_PRINTF_CHECK_INCLUDED
#define Y_PRINTF_CHECK_INCLUDED 1

//! \warning for C++ methods, increase count by 1!
#if defined(__GNUC__)
#	define Y_PRINTF_CHECK(i,j) __attribute__ ((format (printf, i, j)))
#else
#	define Y_PRINTF_CHECK(i,j)
#endif


#endif
