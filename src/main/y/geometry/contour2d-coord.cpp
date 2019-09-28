#include "y/geometry/contour2d.hpp"
#include "y/type/bzset.hpp"

namespace upsylon {

    namespace geometry {

        contour2d:: coordinate:: coordinate(const unit_t I,
                                            const unit_t J,
                                            const short  Q) throw() :
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

        void contour2d:: coordinate:: __run(hashing::function &H) const throw()
        {
            H.run_type(i);
            H.run_type(j);
            H.run_type(q);
        }

        void contour2d:: coordinate:: __sto( unit_t *target ) const throw()
        {
            assert(target);
            target[0] = i;
            target[1] = j;
            target[2] = q;
        }


        int  contour2d:: coordinate:: compare(const coordinate &lhs, const coordinate &rhs) throw()
        {
            unit_t L[3], R[3];
            lhs.__sto(L);
            rhs.__sto(R);
            return comparison::increasing_lexicographic(L,R,3);
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
