#define Y_MATH_IN_TYPES_CXX 1
#include "y/math/ztype.hpp"
#include "y/math/types.hpp"
#include "y/type/xnumeric.hpp"

namespace upsylon
{
	namespace math
	{
        template <> const real_t numeric<real_t>::minimum      = REAL_MIN;
        template <> const real_t numeric<real_t>::epsilon      = REAL_EPSILON;
        template <> const real_t numeric<real_t>::maximum      = REAL_MAX;
        template <> const size_t numeric<real_t>::mant_dig   = REAL_MANT_DIG;
        template <> const real_t numeric<real_t>::pi           = REAL_PI;
        template <> const real_t numeric<real_t>::two_pi       = REAL_TWO_PI;
        template <> const real_t numeric<real_t>::half_pi      = REAL_PI*REAL(0.5);
        template <> const real_t numeric<real_t>::min_exp      = REAL_MIN_EXP;
        template <> const real_t numeric<real_t>::max_exp      = REAL_MAX_EXP;
        template <> const size_t numeric<real_t>::dig          = REAL_DIG;
        template <> const int    numeric<real_t>::min_10_exp   = REAL_MIN_10_EXP;
        template <> const int    numeric<real_t>::max_10_exp   = REAL_MAX_10_EXP;
        template <> const real_t numeric<real_t>::ftol         = REAL(pow)( REAL(10.0), -real_t(dig) );
        template <> const real_t numeric<real_t>::sqrt_ftol    = REAL(pow)( REAL(10.0), -real_t(dig/2+1) );
        template <> const real_t numeric<real_t>::tiny         = REAL(pow)( REAL(10.0), REAL_MIN_10_EXP);
        template <> const real_t numeric<real_t>::huge         = REAL(pow)( REAL(10.0), REAL_MAX_10_EXP);
        template <> const real_t numeric<real_t>::gold         = REAL(0.5) * ( REAL(sqrt)( REAL(5.0) ) + REAL(1.0) );


        real_t atan2_of( real_t y, real_t x )
        {
            static const real_t full_pi = numeric<real_t>::pi;
            static const real_t half_pi = numeric<real_t>::half_pi;

            if( x > 0 )
            {
                return REAL(atan)(y/x);
            }
            else
            {
                if( x < 0 )
                {
                    return (y >= 0) ? REAL(atan)(y/x) + full_pi : REAL(atan)(y/x) - full_pi;
                }
                else
                {
                    // undefined for (0,0) => set 0
                    return y > 0 ? half_pi : ( y < 0 ? -half_pi : 0 );
                }
            }
        }

        real_t angle_of( real_t c, real_t s ) throw()
        {
            std::cerr << "c=" << c << ", s=" << s << std::endl;
            const real_t den = hypotenuse_of(c,s); assert(den>REAL(0.0));
            const real_t C   = c/den;
            const real_t A   = REAL(acos)(C);
            if(s>=0)
            {
                return A;
            }
            else
            {
                return numeric<real_t>::two_pi - A;
            }
        }

	}

    template <> math::real_t xnumeric<math::real_t>::abs_minimum() { return math::numeric<math::real_t>::minimum; }


}
