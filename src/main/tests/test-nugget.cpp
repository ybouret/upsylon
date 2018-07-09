#include "y/memory/nuggets.hpp"
#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace  {
    struct block
    {
        void *addr;
    };

    template <const size_t BLOCK_BITS>
    static inline void do_test()
    {
        const size_t             num_max = 100;
        memory::cblock_of<block> blk(num_max);
        block *b = blk.data;

        for(size_t num_blocks=1;num_blocks<=num_max;++num_blocks)
        {
            const size_t chunk_size = (1<<BLOCK_BITS)*num_blocks;
            memory::cblock_of<char> chunk_mem(chunk_size);
            memory::nugget<BLOCK_BITS> Nugget(chunk_size,chunk_mem.data);
            assert(Nugget.provided_number==num_blocks);

            for(size_t iter=0;iter<128;++iter)
            {
                for(size_t i=0;i<num_blocks;++i)
                {
                    b[i].addr = Nugget.acquire();
                }
                alea.shuffle(b,num_blocks);
                for(size_t i=0;i<num_blocks;++i)
                {
                    Nugget.release(b[i].addr);
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
    __NUGGET(4);
    __NUGGET(5);
    __NUGGET(6);
    __NUGGET(7);
    __NUGGET(8);

}
Y_UTEST_DONE()

Y_UTEST(nuggets)
{
    memory::nuggets<3> Nugs;
    Nugs.acquire();
}
Y_UTEST_DONE()

