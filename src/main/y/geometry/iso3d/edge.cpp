#include "y/geometry/iso3d/edge.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {


            Edge::~Edge() throw()
            {
            }
            
            Edge:: Edge( const Coordinate &C ) throw() :
            lower(C),
            upper(C)
            {
            }

            Edge:: Edge(const Coordinate &a, const Coordinate &b) throw() :
            lower(a),
            upper(b)
            {
                assert(a!=b);
                if( Coordinate::Compare(a, b) >= 0 )
                {
                    bswap(lower,upper);
                }
                assert( Coordinate::Compare(a,b) < 0 );
            }

            Edge:: Edge( const Edge &_) throw() :
            lower( _.lower ),
            upper( _.upper )
            {
            }

            int Edge:: Compare(const Edge &lhs, const Edge &rhs) throw()
            {
                assert(Coordinate::Compare(lhs.lower,lhs.upper) <= 0 );
                assert(Coordinate::Compare(rhs.lower,rhs.upper) <= 0 );

                

            }

            Edge:: Hasher:: Hasher() throw() : H() {}
            Edge:: Hasher:: ~Hasher() throw() {}

            size_t Edge::Hasher::operator()(const Edge &_) throw()
            {
                H.set();
                _.lower.__run(H);
                _.upper.__run(H);
                return H.key<size_t>();
            }


        }

    }

}
