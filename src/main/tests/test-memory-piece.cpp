#include "y/memory/small/piece.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/utils.hpp"
using namespace upsylon;
using namespace memory;

Y_UTEST(piece)
{
    Y_UTEST_SIZEOF(small::piece);

    global &mgr = global::instance();

    for(size_t block_size=1;block_size<=40;++block_size)
    {
        std::cerr << "<block_size=" << block_size << ">" << std::endl;
        const size_t max_chunk_size =  small::piece::max_chunk_size_for(block_size);
        std::cerr << "\tmax_chunk_size=" << max_chunk_size << std::endl;

        for(size_t usr_chunk_size=next_power_of_two(block_size);usr_chunk_size<=8192;usr_chunk_size<<=1)
        {
            const size_t chunk_size = min_of( usr_chunk_size, max_chunk_size);

            void        *chunk_data = mgr.__calloc(1,chunk_size);
            small::piece p(block_size,chunk_data,chunk_size);
            std::cerr << "\tchunk_size: " << usr_chunk_size << " -> " << chunk_size << " => #blocks=" << int(p.provided_number) << " in " << (p.last-p.data) << " bytes" << std::endl;
            mgr.__free(chunk_data,chunk_size);
        }

        std::cerr << "<block_size=" << block_size << "/>" << std::endl << std::endl;
    }

}
Y_UTEST_DONE()


