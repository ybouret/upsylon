#include "y/memory/chunk.hpp"
#include "y/utest/run.hpp"
#include "y/exceptions.hpp"
#include "y/random/bits.hpp"

using namespace upsylon;

#define __SHOW(X) std::cerr << "sizeof(" #X ")=" << sizeof(X) << std::endl

namespace
{
    struct block_t
    {
        void *addr;
    };

    template <typename CHUNK>
    static inline void do_test_chunk( CHUNK &ch , const size_t block_size )
    {
        random::cstdbits ran;
        const size_t nblock = ch.provided_number;
        std::cerr << "nblock=" << nblock << "/block_size=" << block_size << ", words_increment=" << ch.words_increment << std::endl;
        block_t *blk = (block_t *)calloc(nblock,sizeof(block_t));
        if(!blk) throw libc::exception(errno,"calloc");

        for(size_t iter=0;iter<16;++iter)
        {
            for(size_t i=0;i<nblock;++i)
            {
                blk[i].addr = ch.acquire();
            }
            ran.shuffle(blk,nblock);
            for(size_t i=0;i<nblock;++i)
            {
                memset( blk[i].addr, ran.full<uint8_t>(), block_size);
                ch.release( blk[i].addr );
            }

            for(size_t i=0;i<nblock;++i)
            {
                blk[i].addr = ch.acquire();
            }
            ran.shuffle(blk,nblock);
            const size_t hblock=nblock/2;
            for(size_t i=hblock;i<nblock;++i)
            {
                memset( blk[i].addr, ran.full<uint8_t>(), block_size);
                ch.release( blk[i].addr );
            }
            for(size_t i=hblock;i<nblock;++i)
            {
                blk[i].addr = ch.acquire();
            }
            ran.shuffle(blk,nblock);
            for(size_t i=0;i<nblock;++i)
            {
                memset( blk[i].addr, ran.full<uint8_t>(), block_size);
                ch.release( blk[i].addr );
            }

        }
        free(blk);
    }
}


Y_UTEST(chunk)
{
    
    char data[4096];
    for(size_t block_size=1;block_size<=512;++block_size)
    {
        std::cerr << "block_size=" << block_size << std::endl;
        memory::chunk<uint8_t>  chunk1(block_size,data,sizeof(data));
        do_test_chunk(chunk1,block_size);
        memory::chunk<uint16_t> chunk2(block_size,data,sizeof(data));
        do_test_chunk(chunk2,block_size);

        memory::chunk<uint32_t> chunk4(block_size,data,sizeof(data));
        do_test_chunk(chunk4,block_size);

        memory::chunk<uint64_t> chunk8(block_size,data,sizeof(data));
        do_test_chunk(chunk8,block_size);

    }
    __SHOW(memory::chunk<uint8_t>);
    __SHOW(memory::chunk<uint16_t>);
    __SHOW(memory::chunk<uint32_t>);
    __SHOW(memory::chunk<uint64_t>);
    __SHOW(memory::chunk<size_t>);

}
Y_UTEST_DONE()

