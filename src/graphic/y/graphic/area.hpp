//! \file

#ifndef Y_GRAPHIC_AREA_INCLUDED
#define Y_GRAPHIC_AREA_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {

        class Rectangle;

        class Area : public Object
        {
        public:
            virtual ~Area() throw();
            explicit Area(const size_t W, const size_t H);
            Area(const Area &) throw();
            
            const unit_t w;       //!< internally signed
            const unit_t h;       //!< internally signed
            const size_t items;   //!< w*h

            bool contains(const unit_t i, const unit_t j) const throw();
            bool contains(const Rectangle &) const throw();
            
        private:
            Y_DISABLE_ASSIGN(Area);
        };
    }

}

#endif

