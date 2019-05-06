#include "y/memory/slab.hpp"




namespace upsylon
{
    namespace memory
    {
        slab:: slab(const size_t block_size,
                    void        *chunk_data,
                    const size_t chunk_size) throw() :
        chunk(block_size,chunk_data,chunk_size)
        {
        }
        
        slab:: ~slab() throw()
        {
            const size_t still_allocated = chunk.allocated();
            if(still_allocated>0) { std::cerr << "[memory.slab] still #allocated=" << still_allocated << std::endl; }

        }

        size_t slab:: size() const throw()
        {
            return chunk.still_available;
        }
        
        size_t slab::capacity() const throw()
        {
            return chunk.provided_number;
        }
    }

}
