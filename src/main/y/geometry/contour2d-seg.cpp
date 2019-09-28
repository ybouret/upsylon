#include "y/geometry/contour2d.hpp"

namespace upsylon {

    namespace geometry {

        contour2d:: segment:: segment(const point &h, const point &t) throw() :
        head(h),
        tail(t),
        next(0),
        prev(0)
        {
            assert(head!=tail);
        }

        contour2d:: segment::  segment(const segment &_ ) throw() :
        head(_.head),
        tail(_.tail),
        next(0),
        prev(0)
        {
            assert(head!=tail);
        }

        contour2d:: segment:: ~segment() throw()
        {

        }

        bool contour2d:: segment:: are_the_same(const segment &lhs, const segment &rhs) throw()
        {
            return (lhs.head==rhs.head) && (lhs.tail==rhs.tail);
        }

        bool contour2d:: segment:: are_opposite(const segment &lhs, const segment &rhs) throw()
        {
            return (lhs.head==rhs.tail) && (lhs.tail==rhs.head);
        }


        bool operator==( const  contour2d::segment &lhs, const  contour2d::segment &rhs) throw()
        {
            return contour2d::segment::are_the_same(lhs,rhs) ||  contour2d::segment::are_opposite(lhs,rhs);
        }

        bool operator!=( const  contour2d::segment &lhs, const  contour2d::segment &rhs) throw()
        {
            return !contour2d::segment::are_the_same(lhs,rhs) && !contour2d::segment::are_opposite(lhs,rhs);
        }

    }

}


namespace upsylon {

    namespace geometry {

        contour2d:: segments:: segments() throw()
        {
        }

        contour2d:: segments:: ~segments() throw()
        {
        }

        contour2d:: segments:: segments(const segments &other) : segment::list_type(other)
        {
        }


    }
}
