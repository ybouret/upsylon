#include "y/memory/blocks.hpp"
#include "y/type/utils.hpp"
#include "y/memory/global.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {
        blocks:: ~blocks() throw()
        {
            global::location().__free(htable,chunk_size);
        }

        blocks:: blocks(const size_t the_max_block_size,
                        const size_t the_chunk_size) :
        max_block_size(the_max_block_size),
        chunk_size( max_of(the_chunk_size,min_chunk_size) ),
        htable_maxi(chunk_size/sizeof(arena_list)),
        htable_size( most_significant_bit_mask(htable_maxi) ),
        htable_mask(htable_size-1),
        htable( static_cast<arena_list *>(global::instance().__calloc(1,chunk_size) )),
        pages()
        {
            std::cerr << "max_block_size=" << max_block_size << std::endl;
            std::cerr << "chunk_size    =" << chunk_size  << std::endl;
            std::cerr << "htable_maxi   =" << htable_maxi << std::endl;
            std::cerr << "htable_size   =" << htable_size << std::endl;
            std::cerr << "htable_mask   =" << htable_mask << std::endl;
            
        }
    }
}

