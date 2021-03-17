
//! \file

#ifndef Y_COLOR_SIZE_TO_RGBA_INCLUDED
#define Y_COLOR_SIZE_TO_RGBA_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"

namespace upsylon
{
    namespace graphic
    {

        class size_to_rgba : public type_to_rgba
        {
        public:
            virtual ~size_to_rgba() throw();
            explicit size_to_rgba();
            virtual rgba   operator()(const void *) const throw();
            virtual unit_t depth()                  const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(size_to_rgba);
        };
    }

}

#endif
