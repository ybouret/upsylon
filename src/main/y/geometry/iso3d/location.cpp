#include "y/geometry/iso3d/location.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {


            Location::~Location() throw()
            {
            }
            
            Location:: Location( const Coordinate &C ) throw() :
            lower(C),
            upper(C)
            {
            }

            Location:: Location(const Coordinate &a, const Coordinate &b) throw() :
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

            Location:: Location( const Location &_) throw() :
            lower( _.lower ),
            upper( _.upper )
            {
            }

            Location:: Hasher:: Hasher() throw() : H() {}
            Location:: Hasher:: ~Hasher() throw() {}

            size_t Location::Hasher::operator()(const Location &_) throw()
            {
                H.set();
                _.lower.__run(H);
                _.upper.__run(H);
                return H.key<size_t>();
            }


        }

    }

}
