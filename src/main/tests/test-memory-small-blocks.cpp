

#include "y/memory/small/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(small_blocks)
{


    for(size_t chunk_size=1; chunk_size<=4096; chunk_size<<=1 )
    {
        std::cerr << "<chunk_size=" << chunk_size << ">" << std::endl;
        for(size_t limit_size=1;limit_size<=512;limit_size<<=1)
        {
            std::cerr << "\tlimit_size=" << std::setw(5) << limit_size << ":";
            small::blocks blk(chunk_size,limit_size);
            std::cerr << " | chunk_size="    << std::setw(5) <<  blk.chunk_size;
            std::cerr << " | slots_size="    << std::setw(5) <<  blk.slots_size;
            std::cerr << " | chunks#size,rise="  << std::setw(5) <<  blk.chunks.chunk_size << "," << std::setw(5) <<  blk.chunks.nodes_rise;
            std::cerr << " | arenas#size,rise="  << std::setw(5) <<  blk.arenas.chunk_size << "," << std::setw(5) <<  blk.arenas.nodes_rise;

            std::cerr << std::endl;
        }
        std::cerr << "<chunk_size=" << chunk_size << "/>" << std::endl;
        std::cerr << std::endl;
    }
    
    Y_UTEST_SIZEOF(small::blocks::slot_type);


}
Y_UTEST_DONE()

