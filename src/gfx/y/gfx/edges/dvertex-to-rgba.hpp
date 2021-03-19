
//! \file

#ifndef Y_GFX_EDGES_DVERTEX_TO_RGBA_INCLUDED
#define Y_GFX_EDGES_DVERTEX_TO_RGBA_INCLUDED 1

#include "y/gfx/edges/dvertex.hpp"
#include "y/gfx/color/type-to-rgba.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            //__________________________________________________________________
            //
            //
            //! to save digitized vertices
            //
            //__________________________________________________________________
            class dvertex_to_rgba : public type_to_rgba
            {
            public:
                static const size_t directions = 9; //!< all possible direction

                explicit dvertex_to_rgba() throw(); //!< setup
                virtual ~dvertex_to_rgba() throw(); //!< cleanup

                virtual unit_t depth() const throw();                  //!< sizeof(vertex)
                virtual rgba   operator()(const void *) const throw(); //!< 9 colors

                //! chosen colors
                static const rgba colors[directions];
            private:
                Y_DISABLE_COPY_AND_ASSIGN(dvertex_to_rgba);
            };
        }

    }

}

#endif
