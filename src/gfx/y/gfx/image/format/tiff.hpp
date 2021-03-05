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

            //! compile a bitmap to a RGBA raster
            static void compile(_TIFF::Raster        &raster,
                                const bitmap         &bmp,
                                const type_to_rgba   &proc);
            
            //! expand a RGBA raster into a Bitmap
            static void expand(bitmap              &bmp,
                               const _TIFF::Raster &raster,
                               const rgba_to_type  &proc);

        private:
            mutable _TIFF::Raster raster;
            Y_DISABLE_COPY_AND_ASSIGN(tiff_format);
            
            
            virtual bitmap load_(const string         &file,
                                 const image::options *opts,
                                 const rgba_to_type   &conv) const;
            
            virtual void   save_(const bitmap         &bmp,
                                 const string         &file,
                                 const image::options *opts,
                                 const type_to_rgba   &conv) const;
        };

    }

}


#endif


