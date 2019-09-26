
#include "y/geometry/contour2d.hpp"
#include "y/type/bzset.hpp"
#include "y/comparison.hpp"
#include "y/type/bswap.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon {

    namespace geometry {

        double contour:: average( const double v1, const double v2, const double v3, const double v4) throw()
        {
            double a[4] = { v1, v2, v3, v4 };
            lightweight_array<double> arr(a,sizeof(a)/sizeof(a[0]));
            return sorted_sum_abs(arr) * 0.25;
        }

    }

}


namespace upsylon {

    namespace geometry {

        contour2d:: coordinate:: coordinate(const unit_t I,
                                            const unit_t J,
                                            const unit_t Q) throw() :
        i(I), j(J), q(Q)
        {
        }

        contour2d:: coordinate:: ~coordinate() throw()
        {
            bzset_(i);
            bzset_(j);
            bzset_(q);
        }

        contour2d:: coordinate:: coordinate( const coordinate &_ ) throw() :
        i( _.i ),
        j( _.j ),
        q( _.q )
        {
        }

        int  contour2d:: coordinate:: compare(const coordinate &lhs, const coordinate &rhs) throw()
        {
            return comparison::increasing_lexicographic(&lhs.i,&rhs.i,3);
        }

        bool operator==(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.q==rhs.q);
        }

        bool operator!=(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return (lhs.i!=rhs.i) || (lhs.j!=rhs.j) || (lhs.q!=rhs.q);
        }

        std::ostream & operator<<(std::ostream &os,const contour2d::coordinate&c)
        {
            os << '(' << c.i << ',' << c.j << ':' << c.q << ')';
            return os;
        }

        bool operator<(const contour2d::coordinate &lhs, const contour2d::coordinate &rhs) throw()
        {
            return contour2d::coordinate::compare(lhs,rhs) < 0;
        }

    }

}

namespace upsylon {

    namespace geometry {


        contour2d:: edge:: ~edge() throw()
        {
        }

        contour2d:: edge:: edge( const coordinate &single) throw() :
        lower(single),
        upper(single)
        {
        }

        contour2d:: edge:: edge( const coordinate &a, const coordinate &b) throw() :
        lower(a),
        upper(b)
        {
            assert(a!=b);
            if( coordinate::compare(lower,upper) > 0 )
            {
                bswap(lower,upper);
            }
            assert(coordinate::compare(lower,upper)<0);

        }

        bool operator==(const contour2d::edge&lhs,const contour2d::edge&rhs) throw()
        {
            return (lhs.lower==rhs.lower) && (lhs.upper==rhs.upper);
        }

        bool operator!=(const contour2d::edge&lhs,const contour2d::edge&rhs) throw()
        {
            return (lhs.lower!=rhs.lower) || (lhs.upper!=rhs.upper);
        }

    }

}

