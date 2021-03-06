#include "y/gfx/stack.hpp"
#include "y/gfx/image/io.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        namespace crux
        {
            static const char    fn[] = "graphic::stack: ";

            void stack:: check_tiff_handles(const string &filename)
            {
                static image::io    &img  = image::io::instance().standard();
                image::named_format *fmt  = img(tiff_format::ID);
                if(!fmt)
                {
                    throw exception("%smissing %s!!!",fn,tiff_format::ID);
                }
                if(!fmt->handles(filename))
                {
                    throw exception("%sinvalid %s file name '%s'",fn,tiff_format::ID,*filename);
                }
            }
            
            void stack:: check_same_metrics(const area &a, I_TIFF &tiff, const string &filename)
            {
                const unit_t w = tiff.GetWidth();
                const unit_t h = tiff.GetHeight();
                if(a.w!=w||a.h!=h)
                    throw exception("%smismatch %ldx%ld != %ldx%ld for '%s'",fn, long(a.w), long(a.h), long(w), long(h), *filename);
            }

            size_t stack:: min_directories(const size_t nmax, const string &filename)
            {
                const size_t ndir = I_TIFF::CountDirectoriesOf(filename);
                if(nmax>0)
                {
                    return min_of(nmax,ndir);
                }
                else
                {
                    return ndir;
                }
            }

        }
        
    }
}
