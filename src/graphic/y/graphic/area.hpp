//! \file

#ifndef Y_GRAPHIC_AREA_INCLUDED
#define Y_GRAPHIC_AREA_INCLUDED 1

#include "y/graphic/types.hpp"

namespace upsylon {

    namespace Graphic {
        
        class Area : public Object
        {
        public:
            virtual ~Area() throw();
            explicit Area(const size_t W, const size_t H);

            const size_t w;
            const size_t h;
            const size_t items;
            
        private:
            Y_DISABLE_ASSIGN(Area);
        };
    }

}

#endif

