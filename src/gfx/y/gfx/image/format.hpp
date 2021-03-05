//! \file

#ifndef Y_GFX_IMAGE_FORMAT_INCLUDED
#define Y_GFX_IMAGE_FORMAT_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/color/type-to-rgba.hpp"
#include "y/gfx/color/rgba-to-type.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {
            
            //__________________________________________________________________
            //
            //! format object: match/load/save
            //__________________________________________________________________
            class format : public entity
            {
            public:
                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual     ~format() throw();                     //!< cleanup
                virtual bool matches(const string &ext) const = 0; //!< exact extension math

                //! load a new bitmap
                virtual bitmap load(const string       &file,
                                    const void         *opts,
                                    const rgba_to_type &conv) = 0;

                //! save a bitmap
                virtual void   save(const bitmap       &bmp,
                                    const string       &file,
                                    const void         *opts,
                                    const type_to_rgba &conv) = 0;

                //______________________________________________________________
                //
                // non-virtual interface
                //______________________________________________________________

                //! handles file if matches extension
                template <typename FILENAME>
                bool handles(const FILENAME &filename) const
                {
                    const string ext = vfs::get_extension(filename);
                    return matches(ext);
                }

            protected:
                explicit format() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(format);
            };

        }

    }

}

#endif
