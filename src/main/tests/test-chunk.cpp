#include "y/memory/chunk.hpp"
#include "y/utest/run.hpp"
#include "y/exceptions.hpp"

using namespace upsylon;

#define __SHOW(X) std::cerr << "sizeof(" #X ")=" << sizeof(X) << std::endl

namespace
{
    struct block_t
    {
        void *addr;
    };

    template <typename CHUNK>
    static inline void do_test_chunk( CHUNK &ch )
    {
        const size_t nblock = ch.provided_number;
        std::cerr << "nblock=" << nblock << std::endl;
        block_t *blk = (block_t *)calloc(nblock,sizeof(block_t));
        if(!blk) throw libc::exception(errno,"calloc");

        for(size_t i=0;i<nblock;++i)
        {
            blk[i].addr = ch.acquire();
        }


        free(blk);
    }
}


Y_UTEST(chunk)
{

    char data[1024];
    for(size_t block_size=1;block_size<=8;++block_size)
    {
        std::cerr << "block_size=" << block_size << std::endl;
        memory::chunk<uint8_t>  chunk1(block_size,data,sizeof(data));
        do_test_chunk(chunk1);
        memory::chunk<uint16_t> chunk2(block_size,data,sizeof(data));
        do_test_chunk(chunk2);

        memory::chunk<uint32_t> chunk4(block_size,data,sizeof(data));
        do_test_chunk(chunk4);

        memory::chunk<uint64_t> chunk8(block_size,data,sizeof(data));
        do_test_chunk(chunk8);

    }
    __SHOW(memory::chunk<uint8_t>);
    __SHOW(memory::chunk<uint16_t>);
    __SHOW(memory::chunk<uint32_t>);
    __SHOW(memory::chunk<uint64_t>);
    __SHOW(memory::chunk<size_t>);

}
Y_UTEST_DONE()

