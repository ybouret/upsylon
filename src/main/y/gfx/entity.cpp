#include "y/gfx/entity.hpp"
#include "y/memory/embed.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/os/oor.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace graphic
    {
        entity::  entity() throw() : object(), counted() {}
        entity:: ~entity() throw() {}

        void entity:: suppress(void *&wksp, size_t &wlen) throw()
        {
            if(wksp)
            {
                assert(wlen>0);
                static memory::allocator &mgr = memory::dyadic::location();
                out_of_reach::fill(wksp,0,wlen);
                mgr.release(wksp,wlen);
            }
            else
            {
                assert(wlen<=0);
            }
        }

        void * entity:: allocate(memory::embed emb[], const size_t num, size_t &wlen)
        {
            static memory::allocator &mgr = memory::dyadic::instance();
            assert(emb);
            assert(num>0);
            return memory::embed::create(emb, num,mgr,wlen);
        }

        void  entity::error(const char *when)
        {
            assert(when);
            throw exception("%s",when);
        }

    }

}

