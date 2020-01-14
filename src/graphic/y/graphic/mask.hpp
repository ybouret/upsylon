//! \file

#ifndef Y_GRAPHIC_MASK_INCLUDED
#define Y_GRAPHIC_MASK_INCLUDED 1

#include "y/graphic/types.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon {

    namespace Graphic {

        //! lexicographic point comparator
        class PointComparator
        {
        public:
            PointComparator()  throw(); //!< setup
            ~PointComparator() throw(); //!< cleanup

            //! return lexicographic comparison
            int operator()(const Point &lhs, const Point &rhs) const throw();
           

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PointComparator);
        };

        typedef sorted_vector<Point,PointComparator,memory::global> SortedMaskType; //!< alias
        typedef ordered_single<SortedMaskType>                      MaskType;       //!< alias

        //! a Mask is a collection of unique points
        class Mask : public MaskType
        {
        public:
            explicit Mask() throw();       //!< setup
            virtual ~Mask() throw();       //!< cleanup
            explicit Mask(const size_t n); //!< setup with capacity

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mask);
        };

    }

}


#endif

