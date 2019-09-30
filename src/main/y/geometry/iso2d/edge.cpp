#include "y/geometry/iso2d/edge.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            Edge:: ~Edge() throw()
            {
            }

            Edge:: Edge( const Coordinate &single) throw() :
            lower(single),
            upper(single)
            {
            }

            Edge:: Edge( const Edge &_) throw() :
            lower(_.lower),
            upper(_.upper)
            {
            }

            Edge:: Edge( const Coordinate &a, const Coordinate &b) throw() :
            lower(a),
            upper(b)
            {
                assert(a!=b);
                if( Coordinate::Compare(lower,upper) > 0 )
                {
                    bswap(lower,upper);
                }
                assert(Coordinate::Compare(lower,upper)<0);

            }

            bool operator==(const Edge&lhs,const Edge&rhs) throw()
            {
                return (lhs.lower==rhs.lower) && (lhs.upper==rhs.upper);
            }

            bool operator!=(const Edge&lhs,const Edge&rhs) throw()
            {
                return (lhs.lower!=rhs.lower) || (lhs.upper!=rhs.upper);
            }

            Edge:: Hasher::  Hasher() throw() : H() {}
            Edge:: Hasher:: ~Hasher() throw()  {}

            size_t Edge:: Hasher::operator()(const Edge &_) throw()
            {
                H.set();
                _.lower.__run(H);
                _.upper.__run(H);
                return H.key<size_t>();
            }


        }
    }
}
