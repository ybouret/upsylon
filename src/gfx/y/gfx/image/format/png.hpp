

//! \file

#ifndef Y_GFX_IMAGE_PNG_INCLUDED
#define Y_GFX_IMAGE_PNG_INCLUDED 1

#include "y/gfx/image/named-format.hpp"

namespace upsylon
{
    namespace graphic
    {

        //! using jpeg
        class png_format : public image::named_format
        {
        public:
            static  const char * const ID; //!< "PNG"
            virtual ~png_format() throw();
            explicit png_format();
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(png_format);

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
