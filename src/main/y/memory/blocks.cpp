#include "y/memory/blocks.hpp"
#include "y/type/utils.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {
        blocks:: ~blocks() throw()
        {
        }

        blocks:: blocks(const size_t the_max_block_size,
                        const size_t the_chunk_size) :
        max_block_size(the_max_block_size),
        chunk_size( max_of(the_chunk_size,min_chunk_size) ),
        htable_maxi(chunk_size/sizeof(arena_list)),
        htable_size( most_significant_bit_mask(htable_maxi) ),
        htable_mask(htable_size-1),
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

