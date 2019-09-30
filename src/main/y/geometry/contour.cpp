#include "y/geometry/contour.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon {

    namespace geometry {

        double Contour:: Average( const double v1, const double v2, const double v3, const double v4) throw()
        {
            double a[4] = { v1, v2, v3, v4 };
            lightweight_array<double> arr(a,sizeof(a)/sizeof(a[0]));
            return sorted_sum_abs(arr) * 0.25;
        }

        Contour::SignType Contour:: SignOf( const double value ) throw()
        {
            if(value<0.0)
            {
                return IsNegative;
            }
            else if(0.0<value)
            {
                return IsPositive;
            }
            else
            {
                return IsZero;
            }
        }

    }

}
