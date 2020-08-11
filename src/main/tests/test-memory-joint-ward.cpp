
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
        std::cerr << "-- sample ward" << std::endl;
        joint::ward W(1000);
        std::cerr << W._quarry() << std::endl;
        std::cerr << std::endl;
    }

    {
        std::cerr << "-- sample ward" << std::endl;
        joint::ward W(4000);
        std::cerr << W._quarry() << std::endl;
        std::cerr << std::endl;
    }

    {
        std::cerr << "-- trial ward" << std::endl;
        joint::ward W(4000);
        std::cerr << W << std::endl;
        std::cerr << std::endl;

        block        blk[32];
        const size_t num = sizeof(blk)/sizeof(blk[0]);

        for(size_t i=0;i<num;++i)
        {
            block &b = blk[i];
            b.size   = alea.leq(8000);
            b.addr   = W.acquire_block(b.size);
        }
        std::cerr << "-- full --" << std::endl;
        std::cerr << W << std::endl;
        std::cerr << std::endl;

        alea.shuffle(blk,num);
        for(size_t i=0;i<num/2;++i)
        {
            W.release_block(blk[i].addr, blk[i].size);
        }

        std::cerr << "-- half --" << std::endl;
        std::cerr << W << std::endl;
        std::cerr << std::endl;

        for(size_t i=num/2;i<num;++i)
        {
            W.release_block(blk[i].addr, blk[i].size);
        }

        std::cerr << "-- empty --" << std::endl;
        std::cerr << W << std::endl;
        std::cerr << std::endl;

    }

}
Y_UTEST_DONE()

