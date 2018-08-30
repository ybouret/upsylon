//! \file
#ifndef Y_INK_IMAGE_JPEG_INCLUDED
#define Y_INK_IMAGE_JPEG_INCLUDED 1

#include "y/ink/image.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! jpeg format
        class jpeg_format : public Image::Format
        {
        public:
            //! constructor
            explicit jpeg_format();
            //! destructor
            virtual ~jpeg_format() throw();
            
            virtual bool     lossless() const throw();
            virtual Bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const;
            
            virtual void     save(const string        &filename,
                                  const Bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(jpeg_format);
            virtual const char **extensions() const throw();
        };
    }
}

#endif

