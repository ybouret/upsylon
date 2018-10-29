//! \file
#ifndef Y_INK_DRAW_PUTPIXEL_INCLUDED
#define Y_INK_DRAW_PUTPIXEL_INCLUDED 1

#include "y/ink/pixmaps.hpp"

namespace upsylon
{
    namespace Ink
    {
        namespace Draw
        {
            struct PutPixel
            {
                template <typename T>
                class Copy
                {
                public:
                    const T color;
                    inline Copy(const T &C) : color(C)
                    {
                    }

                    inline ~Copy() throw()
                    {
                    }

                    inline void operator()(Pixmap<T> &pxm, const coord &q)
                    {
                        assert(pxm.has(q));
                        pxm[q] = color;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Copy);
                };
            };
        }
    }

}

#endif

