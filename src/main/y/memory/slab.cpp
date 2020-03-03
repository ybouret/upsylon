#include "y/memory/slab.hpp"
#include "y/memory/chunk.hpp"
#include "y/os/static-check.hpp"
#include "y/type/cswap.hpp"
#include <cstring>



namespace upsylon
{
    namespace memory
    {

        namespace
        {
            typedef __chunk<size_t> chunk_type;
        }

        slab:: slab(const size_t block_size,
                    void        *chunk_data,
                    const size_t chunk_size) throw() :
        impl(0),
        data()
        {
            Y_STATIC_CHECK(sizeof(data)>=sizeof(chunk_type),data_too_small);
            memset(data,0,sizeof(data));
            impl = (void *) &data[0];
            new (impl) chunk_type(block_size,chunk_data,chunk_size);
        }
        
        slab:: ~slab() throw()
        {
            assert(impl);
            const chunk_type &chunk = *(chunk_type *)impl;
            const size_t still_allocated = chunk.allocated();
            if(still_allocated>0) { std::cerr << "[memory.slab] still #allocated=" << still_allocated << std::endl; }
        }

        void slab:: swap_with( slab &other ) throw()
        {
            mswap(data,other.data,sizeof(data));
        }

        size_t slab:: size() const throw()
        {
            assert(impl);
            return ((chunk_type *)impl)->still_available;
        }
        
        size_t slab::capacity() const throw()
        {
            assert(impl);
            return ((chunk_type *)impl)->provided_number;
        }

        void *slab:: acquire_block()
        {
            assert(impl);
            return ((chunk_type *)impl)->acquire();
        }

        void  slab:: release_block(void *p) throw()
        {
            assert(impl);
            ((chunk_type *)impl)->release(p);
        }
    }

}
