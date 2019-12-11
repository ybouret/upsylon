//! \file
#ifndef Y_INK_IMAGE_TIFF_INCLUDED
#define Y_INK_IMAGE_TIFF_INCLUDED 1

#include "y/graphic/image.hpp"
#include "y/graphic/image/tiff++.hpp"

namespace upsylon
{
    namespace Graphic
    {

        //! used libtiff for I/O
        class TIFF_Format : public Image::Format
        {
        public:
            //! constructor
            explicit TIFF_Format();
            //! destructor
            virtual ~TIFF_Format() throw();

            //! load a new bitmap
            virtual Bitmap *load(const string         &filename,
                                 const size_t          depth,
                                 RGBA2Data            &proc,
                                 const Image::Options *params) const;

            //! save a bitmap
            virtual void    save(const string         &filename,
                                 const Bitmap         &bmp,
                                 Data2RGBA            &proc,
                                 const Image::Options *params) const;

            //! count TIFF directories in one file
            size_t countDirectories(const string &filename) const;
            
        private:
            mutable I_TIFF::Raster raster;
            Y_DISABLE_COPY_AND_ASSIGN(TIFF_Format);
        };

    }

}

#endif


