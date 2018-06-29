#include "y/memory/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/memory/cblock.hpp"
#include <cstdlib>

using namespace upsylon;

namespace  {
    struct block
    {
        size_t size;
        void  *addr;
    };
}
Y_UTEST(blocks)
{

    size_t chunk_size = 256;
    if(argc>1)
    {
        chunk_size = atol(argv[1]);
    }
    std::cerr << "sizeof(arena_list)=" << sizeof(memory::blocks::arena_list) << std::endl;
    std::cerr << "sizeof(arena)     =" << sizeof(memory::arena) << std::endl;
    memory::blocks B(chunk_size);

    
    const size_t n = 100;
    memory::cblock_of<block> blk(n);
    for(size_t i=0;i<n;++i)
    {
        blk.data[i].size = 1+alea.leq(100);
        blk.data[i].addr = B.acquire(blk.data[i].size);
    }

}
Y_UTEST_DONE()

