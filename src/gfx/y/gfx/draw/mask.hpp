
//! \file

#ifndef Y_GRAPHIC_DRAW_MASK
#define Y_GRAPHIC_DRAW_MASK 1

#include "y/gfx/draw/putpixel.hpp"
#include "y/gfx/pixmap.hpp"
#include "y/gfx/mask.hpp"

namespace  upsylon {

    namespace graphic {

        namespace draw {

            //! apply function for every mask point
            template <typename T, typename FUNC>
            inline void _masking(pixmap<T>  &pxm,
                                 const mask &msk,
                                 FUNC       &fcn)
            {
                for(mask::const_iterator it=msk.begin();it!=msk.end();++it)
                {
                    const coord p = *it;
                    fcn(pxm[p],p);
                }
            }

            

#define Y_MASKING_ARGS pixmap<T> &pxm, const mask &msk //!< args for masking
#define Y_MASKING_CALL _masking(pxm,msk,proc)          //!< call for hline

            Y_GFX_DRAW_IMPL(masking,Y_MASKING_ARGS,Y_MASKING_CALL)

#undef Y_MASKING_CALL
#undef Y_MASKING_ARGS

        }
    }
}

#endif
