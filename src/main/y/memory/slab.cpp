#include "y/memory/slab.hpp"
#include "y/memory/chunk.hpp"




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
        impl()
        {
            memset(impl,0,sizeof(impl));
            new (impl) chunk_type(block_size,chunk_data,chunk_size);

        }
        
        slab:: ~slab() throw()
        {
            const chunk_type &chunk = *(chunk_type *) &impl[0];
            const size_t still_allocated = chunk.allocated();
            if(still_allocated>0) { std::cerr << "[memory.slab] still #allocated=" << still_allocated << std::endl; }

        }

        size_t slab:: size() const throw()
        {
            return ((chunk_type *) &impl[0])->still_available;
        }
        
        size_t slab::capacity() const throw()
        {
            return ((chunk_type *) &impl[0])->provided_number;
        }

        void *slab:: acquire_block()
        {
            return ((chunk_type *) &impl[0])->acquire();
        }

        void  slab:: release_block(void *p) throw()
        {
            ((chunk_type *) &impl[0])->release(p);
        }
    }

}
