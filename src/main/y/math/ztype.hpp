#ifndef Y_MATH_ZTYPE_INCLUDED
#define Y_MATH_ZTYPE_INCLUDED 1

#include "y/type/complex.hpp"

#include <cfloat>

namespace upsylon
{
	namespace math
	{
		//======================================================================
		//
		// simple precision types
		//
		//======================================================================
		
#undef Y_ZTYPE_OK

#	if (Y_ZTYPE == 's') || (Y_ZTYPE == 'c')
#   define Y_ZTYPE_OK 1
		typedef float           real_t;
		typedef complex<real_t> cplx_t;
		
#		if Y_ZTYPE == 's'
		typedef real_t z_type;
#		endif
		
#		if Y_ZTYPE == 'c'
		typedef cplx_t z_type;
#		endif
		
		
#	define REAL_DIG        FLT_DIG
#	define REAL_EPSILON    FLT_EPSILON
#	define REAL_MANT_DIG   FLT_MANT_DIG
#	define REAL_MAX        FLT_MAX
#	define REAL_MAX_10_EXP FLT_MAX_10_EXP
#	define REAL_MAX_EXP    FLT_MAX_EXP
#	define REAL_MIN        FLT_MIN
#	define REAL_MIN_10_EXP FLT_MIN_10_EXP
#	define REAL_MIN_EXP    FLT_MIN_EXP
#	define REAL(X)         X##f
#	define REAL_PI         3.1415927410125732421875f
#	define REAL_TWO_PI     6.283185482025146484375f
		
		
		
#	endif // s or c
		
		
		//======================================================================
		//
		// double precision types
		//
		//======================================================================
		
#	if (Y_ZTYPE == 'd') || (Y_ZTYPE == 'q' )
#   define Y_ZTYPE_OK 1
		typedef double          real_t;
		typedef complex<real_t> cplx_t;
		
#		if Y_ZTYPE == 'd'
		typedef real_t z_type;
#		endif
		
#		if Y_ZTYPE == 'q'
		typedef cplx_t z_type;
#		endif
		
		
#	define REAL_DIG        DBL_DIG
#	define REAL_EPSILON    DBL_EPSILON
#	define REAL_MANT_DIG   DBL_MANT_DIG
#	define REAL_MAX        DBL_MAX
#	define REAL_MAX_10_EXP DBL_MAX_10_EXP
#	define REAL_MAX_EXP    DBL_MAX_EXP
#	define REAL_MIN        DBL_MIN
#	define REAL_MIN_10_EXP DBL_MIN_10_EXP
#	define REAL_MIN_EXP    DBL_MIN_EXP
#	define REAL(X)         X
#	define REAL_PI         3.141592653589793115997963468544185161590576171875
#	define REAL_TWO_PI     6.28318530717958623199592693708837032318115234375
		
#	endif // d or q
		
		
#	if Y_ZTYPE == 'i'
#   define Y_ZTYPE_OK 1
		typedef unit_t real_t;
#	endif
		
#if !defined(Y_ZTYPE_OK)
#	error	"Y_ZTYPE is not defined or unhandled"
#endif
		
	}
}


#endif
