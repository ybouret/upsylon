#include "y/gfx/stack.hpp"
#include "y/gfx/image/io.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        namespace crux
        {
            void stack:: check_tiff(const string &filename)
            {
                static const char    fn[] = "graphic::stack: ";
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
            
        }
        
    }
}
