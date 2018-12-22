#include "y/geometry/contour2d.hpp"

namespace upsylon
{
    namespace geometry
    {
        // fonctions
        unsigned contour2d:: sign_flag( const double value ) throw()
        {
            if(value<0)      return sign_negative;
            else if(value>0) return sign_positive;
            else             return sign_zero;
        }

        contour2d::point   contour2d::zfind(const point &pa, const double va,
                                            const point &pb, const double vb) throw()
        {
            assert(va*vb<=0);
            const double num = -va;
            const double den = vb-va;
            const double lam = num/den;
            return point( pa.x + lam * (pb.x-pa.x), pa.y + lam * (pb.y-pa.y) );
        }
        
    }
}

