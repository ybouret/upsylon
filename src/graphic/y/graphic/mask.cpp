
#include "y/graphic/mask.hpp"

namespace upsylon {

    namespace Graphic {

        PointComparator::  PointComparator() throw() {}
        PointComparator:: ~PointComparator() throw() {}

        int PointComparator:: operator()(const Point &lhs, const Point &rhs) const throw()
        {
            return comparison::increasing_lexicographic(&lhs.x, &rhs.x, 2);
        }

        Mask:: Mask() throw() : MaskType() {}

        Mask:: Mask(const size_t n) : MaskType(n,as_capacity) {}


        Mask:: ~Mask() throw()
        {
        }
        

    }

}

