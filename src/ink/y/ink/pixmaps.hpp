//! \file
#ifndef Y_INK_PIXMAPS_INCLUDED
#define Y_INK_PIXMAPS_INCLUDED 1

#include "y/ink/pixmap.hpp"
#include "y/ink/color/rgb.hpp"
#include "y/ink/color/yuv.hpp"

namespace upsylon
{
    namespace ink
    {
        typedef pixmap<float>   pixmapf;     //!< floating point pixmap
        typedef pixmap<uint8_t> pixmap1;     //!< pixmap of bytes
        typedef pixmap<RGB>     pixmap3;     //!< pixmap of packed RGB
        typedef pixmap<RGBA>    pixmap4;     //!< pixmap of packed RGBA
        typedef pixmap<YUV>     YUVpixmap;   //!< pixmap of packed YUV
        typedef pixmap<cplx>    pixmapz;     //!< pixmap of complex real


        //! multiple pixmaps of same type and size
        template <typename T>
        class pixmaps : public area, public slots< pixmap<T> >
        {
        public:
            //! constructor
            explicit pixmaps(const size_t num,
                             const size_t W,
                             const size_t H) :
            area( coord(0,0), W, H ),
            slots< pixmap<T> >(num)
            {
                for(size_t i=0;i<num;++i)
                {
                    this->template build<size_t,size_t>(W,H);
                }
            }

            //! destructor
            inline virtual ~pixmaps() throw()
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(pixmaps);
        };
    }

}

#endif
