
//! \file

#ifndef Y_COLOR_SIZE_TO_RGBA_INCLUDED
#define Y_COLOR_SIZE_TO_RGBA_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //! helper to convert a size_t to rgba, use named_color::get
        //______________________________________________________________________
        class size_to_rgba : public type_to_rgba
        {
        public:
            virtual ~size_to_rgba() throw();            //!< cleanup
            size_to_rgba(const size_t=0);               //!< setup with index of color '1'
            size_to_rgba(const size_to_rgba &) throw(); //!< copy

            virtual rgba   operator()(const void *) const throw(); //!< color from size_t
            virtual unit_t depth()                  const throw(); //!< sizeof(size_t)

        private:
            Y_DISABLE_ASSIGN(size_to_rgba);
            const size_t shift;
        };
    }

}

#endif
