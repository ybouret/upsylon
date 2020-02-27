#include "y/information/modulation.hpp"

namespace upsylon {

    namespace Information {

        Modulation:: ~Modulation() throw() {}

        Modulation::  Modulation() throw() {}

        void Modulation:: apply(void *target, const void *source, const size_t length) throw()
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

        void Modulation:: apply(void *target, const size_t length) throw()
        {
            apply(target,target,length);
        }

        void  Modulation:: apply( memory::rw_buffer &buffer ) throw()
        {
            apply( buffer.rw(), buffer.length() );
        }


    }

}

