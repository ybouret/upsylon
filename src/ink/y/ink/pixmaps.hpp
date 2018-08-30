//! \file
#ifndef Y_INK_PIXMAPS_INCLUDED
#define Y_INK_PIXMAPS_INCLUDED 1

#include "y/ink/pixmap.hpp"
#include "y/ink/color/rgb.hpp"
#include "y/ink/color/yuv.hpp"

namespace upsylon
{
    namespace Ink
    {
        typedef Pixmap<float>   PixmapF;     //!< floating point pixmap
        typedef Pixmap<uint8_t> Pixmap1;     //!< pixmap of bytes
        typedef Pixmap<RGB>     Pixmap3;     //!< pixmap of packed RGB
        typedef Pixmap<RGBA>    Pixmap4;     //!< pixmap of packed RGBA
        typedef Pixmap<YUV>     PixmapYUV;   //!< pixmap of packed YUV
        typedef Pixmap<cplx>    PixmapZ;     //!< pixmap of complex real


        //! multiple pixmaps of same type and size
        template <typename T>
        class Pixmaps : public Area, public slots< Pixmap<T> >
        {
        public:
            //! constructor
            explicit Pixmaps(const size_t num,
                             const size_t W,
                             const size_t H) :
            Area( coord(0,0), W, H ),
            slots< Pixmap<T> >(num)
            {
                for(size_t i=0;i<num;++i)
                {
                    this->template build<size_t,size_t>(W,H);
                }
            }

            //! destructor
            inline virtual ~Pixmaps() throw()
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmaps);
        };
    }

}

#endif
