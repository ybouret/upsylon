
#include "y/memory/joint/ward.hpp"
#include "y/utest/run.hpp"


using namespace upsylon;
using namespace memory;


namespace {

    struct block
    {
        void  *addr;
        size_t size;
    };
}

Y_UTEST(joint_ward)
{

    {
        joint::ward W(1000);
        std::cerr << W._quarry() << std::endl;
    }

    {
        joint::ward W(4000);
        std::cerr << W._quarry() << std::endl;
    }

    {
        joint::ward W(4000);
        std::cerr << W._quarry() << std::endl;

        block        blk[128];
        const size_t num = sizeof(blk)/sizeof(blk[0]);

        for(size_t i=0;i<num;++i)
        {
            block &b = blk[i];
            b.size   = alea.leq(8000);
            b.addr   = W.acquire_block(b.size);
        }

        alea.shuffle(blk,num);
        for(size_t i=0;i<num;++i)
        {
            W.release_block(blk[i].addr, blk[i].size);
        }
        
        std::cerr << W << std::endl;
    }

}
Y_UTEST_DONE()

