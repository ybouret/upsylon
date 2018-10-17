//! \file
#ifndef Y_B_SPLINES_INCLUDED
#define T_B_SPLINES_INCLUDED 1

#include "y/sequence/array.hpp"
#include "y/type/utils.hpp"
#include "y/math/types.hpp"

namespace upsylon
{
    namespace math
    {
        //______________________________________________________________________
        //
        //! Return a point on the cubic b-splines: x=0: begin, x=1: end
        //______________________________________________________________________
        template <typename T,typename U>
        inline U Cubic_Bsplines( T x, const array<U> &points ) throw()
        {
            static const T one(1);
            static const T six(6);
            static const T four(4);
            const unit_t num_points   = points.size();
            const unit_t num_segments = num_points + 1;
            unit_t       start_cv     = -2;

            //______________________________________________________________
            //
            // x: 0 -> 1: find the segment
            //______________________________________________________________
            const T      xscaled = clamp<T>(0,x,1)*num_segments;
            const unit_t segment = unit_t(__floor( xscaled ));
            const T      t       = (x*num_segments-segment);

            //______________________________________________________________
            //
            // prepare points
            //______________________________________________________________
            start_cv += segment;

            const T t2 = t*t;
            const T it = one-t;
            const T t3 = t * t2;

            //______________________________________________________________
            //
            // calculate blending functions for cubic bspline
            //______________________________________________________________
            const T b0 = it*it*it/six;
            const T b1 = (3*t3 - 6*t2 + four)/six;
            const T b2 = (-3*t3 +3*t2 + 3*t + one)/six;
            const T b3 =  t3/six;

            return
            b0 * points[clamp<unit_t>(1,start_cv+1,num_points)] +
            b1 * points[clamp<unit_t>(1,start_cv+2,num_points)] +
            b2 * points[clamp<unit_t>(1,start_cv+3,num_points)] +
            b3 * points[clamp<unit_t>(1,start_cv+4,num_points)];
        }
    }
}

#endif
