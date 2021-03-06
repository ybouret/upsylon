#include "y/geometry/contour.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/sequence/array.hpp"

namespace upsylon {

    namespace geometry {

        double Contour:: Average( const double v1, const double v2, const double v3, const double v4) throw()
        {
            double a[4] = { v1, v2, v3, v4 };
            lightweight_array<double> arr(a,sizeof(a)/sizeof(a[0]));
            return sorted_sum_by_abs(arr) * 0.25;
        }

        double Contour:: Average(const double *d) throw()
        {
            double a[8] = { d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]  };
            lightweight_array<double> arr(a,sizeof(a)/sizeof(a[0]));
            return sorted_sum_by_abs(arr) * 0.125;
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
