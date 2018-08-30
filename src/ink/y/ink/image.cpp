#include "y/ink/image.hpp"
#include "y/exception.hpp"
#include "y/string/convert.hpp"
#include "y/fs/vfs.hpp"

namespace upsylon
{
    namespace  Ink
    {
        Image:: ~Image() throw()
        {
        }

        Image:: Image() :
        ImageIO("image"),
        fmt_db( ),
        ext_db( )
        {
        }

        void Image:: enroll( const Format::Pointer &fmt )
        {
            static const char fn[] = "image::enroll";
            //std::cerr << "enrolling " << fmt->name << std::endl;

            //__________________________________________________________________
            //
            // enroll in fmt_db
            //__________________________________________________________________
            const string &id = fmt->name;
            if( ! fmt_db.insert(fmt) )
            {
                throw exception("%s(multiple '%s')",fn,*id);
            }

            //__________________________________________________________________
            //
            // register by extension
            //__________________________________________________________________
            for(const char **exts = fmt->extensions(); *exts; ++exts )
            {
                string ext = *exts;
                string_convert::to_lower(ext);
                //std::cerr << "|_" << ext << std::endl;
                if( !ext_db.insert(ext,fmt) )
                {
                    throw exception("%s('%s' already register in format '%s')", fn, *ext, *id);
                }
            }
        }

        Image::Format & Image::get_format_for(const string &filename) const
        {
            string ext = vfs::get_extension(filename);
            string_convert::to_lower(ext);
            const Format::Pointer *ppFmt = ext_db.search(ext);
            if(!ppFmt)
            {
                throw exception("no format matching extension '%s'", *ext);
            }
            Image::Format::Pointer proxy( *ppFmt );
            //std::cerr << "using [" << proxy->name << "] for " << filename << std::endl;
            return *proxy;
        }


        Bitmap  *Image::load(const string          &filename,
                             unit_t                 depth,
                             rgba2data             &proc,
                             const void            *options) const
        {
            return get_format_for(filename).load(filename,depth,proc,options);
        }
        
        void     Image:: save(const string        &filename,
                              const Bitmap        &bmp,
                              data2rgba           &proc,
                              const void          *options) const
        {
            get_format_for(filename).save(filename,bmp,proc,options);
        }

        void Image:: display() const
        {
            std::cerr << "<supported extensions>" << std::endl;
            for(Format::ExtTable::const_iterator i=ext_db.begin(); i!=ext_db.end();++i)
            {
                std::cerr << "|_" << i.key() << std::endl;
            }
            std::cerr << "<supported extensions/>" << std::endl;
        }

    }
}
