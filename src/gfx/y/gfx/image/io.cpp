
#include "y/gfx/image/io.hpp"


namespace upsylon
{
    Y_SINGLETON_IMPL(graphic::image::io);

    namespace graphic
    {

        namespace image
        {
            io:: io() : named_format::database(8,as_capacity)
            {
            }

            io:: ~io() throw()
            {
            }

            void  io:: define( named_format *fmt )
            {
                assert(fmt);
                const named_format::pointer ptr(fmt);
                if(!insert(ptr))
                {
                    throw exception("%s: multiple '%s'", call_sign, *(ptr->name));
                }
            }

            bool  io:: matches(const string &ext) const
            {
                for(const_iterator it=begin();it!=end();++it)
                {
                    if( (**it).matches(ext) ) return true;
                }
                return false;
            }

            bitmap io:: load(const string       &file,
                             const void         *opts,
                             const rgba_to_type &conv)
            {
                const string ext = vfs::get_extension(file);
                for(iterator it=begin();it!=end();++it)
                {
                    named_format &fmt = **it;
                    if(fmt.matches(ext))
                    {
                        return fmt.load(file,opts,conv);
                    }
                }
                throw exception("%s.load: unsupported extension '%s'", call_sign, *ext);
            }


            void  io:: save(const bitmap       &bmp,
                            const string       &file,
                            const void         *opts,
                            const type_to_rgba &conv)
            {
                const string ext = vfs::get_extension(file);
                for(iterator it=begin();it!=end();++it)
                {
                    named_format &fmt = **it;
                    if(fmt.matches(ext))
                    {
                        fmt.save(bmp,file,opts,conv);
                        return;
                    }
                }
                throw exception("%s.save: unsupported extension '%s'", call_sign, *ext);
            }


        }
    }

}


