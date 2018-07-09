#include "y/memory/nugget.hpp"
#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace  {
    template <const size_t BLOCK_BITS>
    static inline void do_test()
    {
        for(size_t block_bits=BLOCK_BITS;block_bits<=10;++block_bits)
        {
            const size_t block_size = (1<<block_bits);
            for(size_t num=1;num<=32;++num)
            {
                const size_t chunk_size = num*block_size;
                memory::cblock_of<char> blk(chunk_size);
                memory::nugget<BLOCK_BITS> nn(chunk_size,blk.data);

                while(nn.still_available)
                {
                    (void) nn.acquire();
                }

            }
        }
    }

}

#define __NUGGET(BLOCK_BITS) std::cerr << "sizeof(memory::nugget<" #BLOCK_BITS ">)\t= " << sizeof(memory::nugget<BLOCK_BITS>) << std::endl;\
do_test<BLOCK_BITS>()

Y_UTEST(nugget)
{
    __NUGGET(3);
    
}
Y_UTEST_DONE()

