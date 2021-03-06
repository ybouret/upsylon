#include "y/geometry/iso3d/coordinate.hpp"
#include "y/comparison.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {


            int Coordinate:: Compare(const Coordinate &lhs,const Coordinate &rhs) throw()
            {
#if 0
                static const size_t NU = 4;
                unit_t L[NU] = { 0 };
                unit_t R[NU] = { 0 };
                lhs.__sto(L);
                rhs.__sto(R);
#endif
                return comparison::increasing_lexicographic(&lhs.i,&rhs.i,3);
            }


            Coordinate:: Coordinate(const unit_t I,
                                    const unit_t J,
                                    const unit_t K) throw() :
            i(I), j(J), k(K)
            {

            }

            Coordinate:: Coordinate( const Coordinate &_ ) throw() :
            i(_.i), j(_.j), k(_.k)
            {
            }
            

            Coordinate:: ~Coordinate() throw()
            {
            }

            bool operator==(const Coordinate &lhs, const Coordinate &rhs) throw()
            {
                return (lhs.i==rhs.i) && (lhs.j==rhs.j) && (lhs.k==rhs.k);
            }

            bool operator!=(const Coordinate &lhs, const Coordinate &rhs) throw()
            {
                return (lhs.i!=rhs.i) || (lhs.j!=rhs.j) || (lhs.k!=rhs.k);
            }

            bool operator<(const Coordinate &lhs,const Coordinate &rhs) throw()
            {
                return Coordinate::Compare(lhs,rhs) < 0;
            }


            void Coordinate:: __run( hashing::function &H ) const throw()
            {
                H.run_type(i);
                H.run_type(j);
                H.run_type(k);
            }

#if 0
            void Coordinate:: __sto( unit_t *a ) const throw()
            {
                a[0] = i;
                a[1] = j;
                a[2] = k;
                a[3] = q;
            }
#endif

        }
    }

}
