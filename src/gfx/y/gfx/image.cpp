#include "y/gfx/image.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    Y_SINGLETON_IMPL(graphic::image::io);
    
    namespace graphic
    {
        image:: io:: io() :
        named_format::database(8,as_capacity)
        {
        }

        image:: io:: ~io() throw()
        {
        }

        void image::io:: define( named_format *fmt )
        {
            assert(fmt);
            const named_format::pointer ptr(fmt);
            if(!insert(ptr))
            {
                throw exception("%s: multiple '%s'", call_sign, *(ptr->name));
            }
        }

        bool image::io:: matches(const string &ext) const
        {
            for(named_format::database::const_iterator it=begin();it!=end();++it)
            {
                if( (**it).matches(ext) ) return true;
            }
            return false;
        }

        bitmap image::io:: load(const string       &file,
                                const void         *opts,
                                const rgba_to_type &conv)
        {
            const string ext = vfs::get_extension(file);
            for(named_format::database::iterator it=begin();it!=end();++it)
            {
                named_format &fmt = **it;
                if(fmt.matches(ext))
                {
                    return fmt.load(file,opts,conv);
                }
            }
            throw exception("%s: unsupported extension '%s'", call_sign, *ext);
        }


    }

}
