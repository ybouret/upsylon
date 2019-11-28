
//! \file

#ifndef Y_GRAPHIC_RECTANGLE_INCLUDED
#define Y_GRAPHIC_RECTANGLE_INCLUDED 1

#include "y/graphic/area.hpp"

namespace upsylon {

    namespace Graphic {

        //! a never empty rectangle
        class Rectangle : public Area
        {
        public:
            //! setup with indices reordering
            explicit Rectangle(unit_t x0,
                               unit_t y0,
                               unit_t x1,
                               unit_t y1 ) throw();

            //! setup with indices reordering
            explicit Rectangle( Point lo, Point hi) throw();

            //! cleanup
            virtual ~Rectangle() throw();

            //! no-throw copy
            Rectangle(const Rectangle &other) throw();
            
            const Point lower; //!< lower coordinates
            const Point upper; //!< upper coordinates

            //! display
            friend std::ostream & operator<<( std::ostream &, const Rectangle & );

        private:
            Y_DISABLE_ASSIGN(Rectangle);
        };

    }

}

#endif

