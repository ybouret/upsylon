
//! \file

#ifndef Y_GRAPHIC_BOX_INCLUDED
#define Y_GRAPHIC_BOX_INCLUDED 1

#include "y/graphic/area.hpp"

namespace upsylon {

    namespace Graphic {

        //! a never empty rectangle
        class Box : public Area
        {
        public:
            //! setup with indices reordering
            explicit Box(unit_t x0,
                         unit_t y0,
                         unit_t x1,
                         unit_t y1 ) throw();

            //! setup with indices reordering
            explicit Box( Point lo, Point hi) throw();

            //! cleanup
            virtual ~Box() throw();

            //! no-throw copy
            Box(const Box &other) throw();
            
            const Point lower; //!< lower coordinates
            const Point upper; //!< upper coordinates

            //! display
            friend std::ostream & operator<<( std::ostream &, const Box & );

            //! taste equality of lower and upper
            bool  sameBoxThan( const Box &rect ) const throw();

        private:
            Y_DISABLE_ASSIGN(Box);
        };

    }

    //! declare local helpers
#define Y_GRAPHIC_BOX_LIMITS(A)     \
const unit_t xmin = (A).lower.x; \
const unit_t xmax = (A).upper.x; \
const unit_t ymin = (A).lower.y; \
const unit_t ymax = (A).upper.y

}

#endif

