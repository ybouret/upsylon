
#include "y/graphic/image.hpp"
#include "y/exception.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon {

    namespace Graphic {


        Image:: ~Image() throw()
        {
            
        }


        void Image:: use( Format *format )
        {
            static const char fn[] = "Graphic::Image::use: ";
            if(!format) throw exception("%s NULL format)",fn);
            const Format::Pointer fmt( format );
            if(!formats.insert(fmt))
            {
                throw exception("%s multiple format name '%s'", fn, *(fmt->name));
            }
            compile();
        }


        void Image:: compile()
        {
            db.free();
            for( Format::Set::iterator i=formats.begin();i!=formats.end();++i)
            {
                const Fmt fmt = &(**i);
                for( OrderedStrings::iterator j=fmt->extensions.begin(); j!=fmt->extensions.end();++j)
                {
                    string ext = *j;
                    string_convert::to_lower(ext);
                    if( !db.insert(ext, fmt) )
                    {
                        throw exception("Image::Format: %s has multiple extension '%s'", *(fmt->name), *ext);
                    }
                }
            }
        }

        const Image::Format & Image::FormatFor(const string &filename) const
        {
            string       ext = vfs::get_extension(filename);
            string_convert::to_lower(ext);
            const Fmt   *fmt = db.search(ext);
            if(!fmt) throw exception("Image:: no Format for extension <%s>", *ext);
            return **fmt;
        }

        Bitmap * Image:: load(const string  &filename,
                              const size_t  depth,
                              RGBA2Data     &proc,
                              const Options *params) const
        {
            return FormatFor(filename).load(filename, depth, proc, params);
        }

        void  Image::  save(const string  &filename,
                            const Bitmap  &bmp,
                            Data2RGBA     &proc,
                            const Options *params) const
        {
            FormatFor(filename).save(filename,bmp,proc,params);
        }

    }
}

#include "y/graphic/image/png.hpp"
#include "y/graphic/image/jpeg.hpp"
#include "y/graphic/image/tiff.hpp"

namespace upsylon {

    namespace Graphic {

        Image:: Image() : formats(4,as_capacity), db(8,as_capacity)
        {
            use( new PNG_Format()  );
            use( new JPEG_Format() );
            use( new TIFF_Format() );
        }
    }
}

