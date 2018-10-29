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
            //! put pixel operators
            struct PutPixel
            {
                //! copy function
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


                //! blend function
                template <typename T>
                class Blend
                {
                public:
                    const T       color;
                    const uint8_t alpha;
                    
                    inline Blend(const T &C, const uint8_t A) : color(C), alpha(A)
                    {
                    }

                    inline ~Blend() throw()
                    {
                    }

                    inline void operator()(Pixmap<T> &pxm, const coord &q)
                    {
                        assert(pxm.has(q));
                        pxm[q] = Pixel<T>::Blend(color,pxm[q],alpha);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Blend);
                };
            };
        }
    }

}

#endif

