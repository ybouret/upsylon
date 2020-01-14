#include "y/geometry/iso2d/coordinate.hpp"
#include "y/type/bzset.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            Coordinate:: Coordinate(const unit_t I,
                                    const unit_t J,
                                    const short  Q) throw() :
            i(I), j(J), q(Q)
            {
            }

            Coordinate:: ~Coordinate() throw()
            {
                _bzset(i);
                _bzset(j);
                _bzset(q);
            }

            Coordinate:: Coordinate( const Coordinate &_ ) throw() :
            i( _.i ),
            j( _.j ),
            q( _.q )
            {
            }

            void  Coordinate:: __run(hashing::function &H) const throw()
            {
                H.run_type(i);
                H.run_type(j);
                H.run_type(q);
            }

            void Coordinate:: __sto( unit_t *target ) const throw()
            {
                assert(target);
                target[0] = i;
                target[1] = j;
                target[2] = q;
            }


            int  Coordinate:: Compare(const Coordinate &lhs, const Coordinate &rhs) throw()
            {
                unit_t L[3], R[3];
                lhs.__sto(L);
                rhs.__sto(R);
                return comparison::increasing_lexicographic(L,R,3);
            }

            bool operator==(const Coordinate &lhs, const Coordinate &rhs) throw()
            {
                return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.q==rhs.q);
            }

            bool operator!=(const Coordinate &lhs, const Coordinate &rhs) throw()
            {
                return (lhs.i!=rhs.i) || (lhs.j!=rhs.j) || (lhs.q!=rhs.q);
            }

            std::ostream & operator<<(std::ostream &os,const Coordinate&c)
            {
                os << '(' << c.i << ',' << c.j << ':' << c.q << ')';
                return os;
            }

            bool operator<(const Coordinate &lhs, const Coordinate &rhs) throw()
            {
                return Coordinate::Compare(lhs,rhs) < 0;
            }


        }
    }

}
