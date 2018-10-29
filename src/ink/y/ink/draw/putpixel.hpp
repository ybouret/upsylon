//! \file
#ifndef Y_INK_DRAW_PUTPIXEL_INCLUDED
#define Y_INK_DRAW_PUTPIXEL_INCLUDED 1

#include "y/ink/pixmaps.hpp"
#include "y/ink/mask.hpp"

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
                    const T color;                          //!< color to be set
                    inline  Copy(const T &C) : color(C) { } //!< setup
                    inline ~Copy() throw() {}               //!< destructor

                    //! set pixel
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
                    const T       color; //!< color to be set
                    const uint8_t alpha; //!< with this alpha level

                    //! setup
                    inline  Blend(const T &C, const uint8_t A) : color(C), alpha(A) {}
                    inline ~Blend() throw() {} //!< destructor

                    //! replace pixel
                    inline void operator()(Pixmap<T> &pxm, const coord &q)
                    {
                        assert(pxm.has(q));
                        pxm[q] = Pixel<T>::Blend(color,pxm[q],alpha);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Blend);
                };

                //! register points into a mask
                class Register
                {
                public:
                    Mask &mask; //!< the target mask

                    inline Register( Mask &target ) : mask(target) {} //!< setuyp
                    inline ~Register() throw() {} //!< destrcutor

                    //! append/overwrite coordinate
                    inline void operator()(Bitmap &, const coord &q)
                    {
                        (void) mask.append(q);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Register);
                };
            };
        }
    }

}

namespace upsylon
{
    namespace Ink
    {
        namespace Draw
        {

        }

    }

}



#endif

