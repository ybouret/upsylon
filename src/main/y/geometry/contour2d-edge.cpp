#include "y/geometry/contour2d.hpp"

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

        contour2d:: edge:: edge( const edge &_) throw() :
        lower(_.lower),
        upper(_.upper)
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

        contour2d:: edge:: hasher::  hasher() throw() : H() {}
        contour2d:: edge:: hasher:: ~hasher() throw()  {}

        size_t contour2d:: edge:: hasher::operator()(const edge &_) throw()
        {
            H.set();
            _.lower.__run(H);
            _.upper.__run(H);
            return H.key<size_t>();
        }

    }

}
