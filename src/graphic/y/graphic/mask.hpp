//! \file

#ifndef Y_GRAPHIC_MASK_INCLUDED
#define Y_GRAPHIC_MASK_INCLUDED 1

#include "y/graphic/types.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon {

    namespace Graphic {

        class PointComparator
        {
        public:
            PointComparator() throw();
            ~PointComparator() throw();

            int operator()(const Point &lhs, const Point &rhs) const throw();
           

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PointComparator);
        };

        typedef sorted_vector<Point,PointComparator,memory::global> SortedMaskType;
        typedef ordered_single<SortedMaskType>                      MaskType;

        class Mask : public MaskType
        {
        public:
            explicit Mask() throw();
            virtual ~Mask() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mask);
        };

    }

}


#endif

