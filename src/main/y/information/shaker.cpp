#include "y/information/shaker.hpp"

namespace upsylon {

    namespace information {

        shaker:: ~shaker() throw() {}

        shaker::  shaker() throw() {}

        void shaker:: apply(void *target, const void *source, const size_t length) throw()
        {
            assert(!(0==target&&length>0));
            assert(!(0==source&&length>0));
            reset();
            uint8_t       *t = static_cast<uint8_t       *>(target);
            const uint8_t *s = static_cast<const uint8_t *>(source);
            for(size_t i=0;i<length;++i)
            {
                t[i] = fetch( s[i] );
            }
        }

        void shaker:: apply(void *target, const size_t length) throw()
        {
            apply(target,target,length);
        }

        void  shaker:: apply( memory::rw_buffer &buffer ) throw()
        {
            apply( buffer.rw(), buffer.length() );
        }


    }

}

