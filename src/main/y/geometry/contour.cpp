#include "y/geometry/contour.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon {

    namespace geometry {

        double contour:: average( const double v1, const double v2, const double v3, const double v4) throw()
        {
            double a[4] = { v1, v2, v3, v4 };
            lightweight_array<double> arr(a,sizeof(a)/sizeof(a[0]));
            return sorted_sum_abs(arr) * 0.25;
        }

        contour::sign_type contour:: sign_of( const double value ) throw()
        {
            if(value<0.0)
            {
                return is_negative;
            }
            else if(0.0<value)
            {
                return is_positive;
            }
            else
            {
                return is_zero;
            }
        }

    }

}
