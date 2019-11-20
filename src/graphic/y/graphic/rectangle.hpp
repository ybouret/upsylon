
//! \file

#ifndef Y_GRAPHIC_RECTANGLE_INCLUDED
#define Y_GRAPHIC_RECTANGLE_INCLUDED 1

#include "y/graphic/area.hpp"

namespace upsylon {

    namespace Graphic {

        class Rectangle : public Area
        {
        public:

            explicit Rectangle( unit_t x0, unit_t y0, unit_t x1, unit_t y1 ) throw();
            virtual ~Rectangle() throw();
            Rectangle(const Rectangle &other) throw();

            const unit_t xmin;
            const unit_t ymin;
            const unit_t xmax;
            const unit_t ymax;

        private:
            Y_DISABLE_ASSIGN(Rectangle);
        };

    }

}

#endif

