#include "y/memory/kchunk.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(kChunk)
{
    char data[1024];
    for(size_t block_size=1;block_size<=512;block_size*=2)
    {
        memory::tChunk<uint8_t>  chunk1(data,block_size,sizeof(data));
        memory::tChunk<uint16_t> chunk2(data,block_size,sizeof(data));

    }
}
Y_UTEST_DONE()

