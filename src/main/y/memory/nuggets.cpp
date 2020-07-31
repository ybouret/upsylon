#include "y/memory/nuggets.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/arena.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{

    namespace memory
    {

        void nuggets:: clr() throw()
        {
            memset(wksp,0,sizeof(wksp));
        }

        nuggets:: ~nuggets() throw()
        {
            self_destruct( *(arena *)( impl) );
            clr();
        }


        nuggets:: nuggets() throw() : impl(0), wksp()
        {
            clr();
            assert(is_a_power_of_two(small_chunk_size));
            new ( impl = (void*)&wksp[0] ) memory::arena( sizeof(nugget<3>), Y_CHUNK_SIZE );
        }

        void * nuggets:: global_calloc( const size_t count, const size_t size)
        {
            static memory::global &G = memory::global::instance();
            return G.__calloc(count,size);
        }

        void nuggets:: global_free(void *p, const size_t bytes) throw()
        {
            assert( memory::global::exists() );
            static memory::global &G = memory::global::location();
            G.__free(p,bytes);
        }

        void * nuggets:: query_nugget_space()
        {
            assert(impl);
            return ((arena*)impl)->acquire();
        }

        void nuggets:: store_nugget_space(void *p) throw()
        {
            assert(p);
            assert(impl);
            ((arena*)impl)->release(p);
        }
    }

}
