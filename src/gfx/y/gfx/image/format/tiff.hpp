//! \file
#ifndef Y_GFX_IMAGE_TIFF_INCLUDED
#define Y_GFX_IMAGE_TIFF_INCLUDED 1

#include "y/gfx/image/named-format.hpp"
#include "y/gfx/image/format/tiff++.hpp"

namespace upsylon
{
    namespace graphic
    {

        //! used libtiff for I/O
        class tiff_format : public image::named_format
        {
        public:
            static const char ID[]; //!< "TIFF"

            //! constructor
            explicit tiff_format();
            //! destructor
            virtual ~tiff_format() throw();

#if 0
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
#endif

            //! compile a bitmap to a RGBA raster
            static void compile(_TIFF::Raster        &raster,
                                const bitmap         &bmp,
                                type_to_rgba         &proc);
            
            //! expand a RGBA raster into a Bitmap
            static void expand(bitmap              &bmp,
                               const _TIFF::Raster &raster,
                               rgba_to_type        &proc);

        private:
            mutable _TIFF::Raster raster;
            Y_DISABLE_COPY_AND_ASSIGN(tiff_format);
        };

    }

}


#endif


