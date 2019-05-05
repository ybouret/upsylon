#include "y/memory/nuggets.hpp"
#include "y/memory/global.hpp"

namespace upsylon
{

    namespace memory
    {
        nuggets:: ~nuggets() throw()
        {
        }


        nuggets:: nuggets() throw()
        {
            assert(is_a_power_of_two(small_chunk_size));
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
    }

}
