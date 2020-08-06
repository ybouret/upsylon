
#include "y/memory/small/hoard.hpp"
#include "y/memory/small/quarry.hpp"
#include "y/memory/small/blocks.hpp"

#include "y/concurrent/fake-lock.hpp"
#include "y/utest/run.hpp"

#include <cstring>

using namespace upsylon;
using namespace memory;

namespace
{
    static inline void doMT( small::hoard &h )
    {
        std::cerr << "block_size=" << h.block_size << std::endl;

        void        *reg[100];
        const size_t num = sizeof(reg)/sizeof(reg[0]);
        memset(reg,0,sizeof(reg));
        for(size_t i=0;i<num;++i)
        {
            reg[i] = h.acquire();
        }
        alea.shuffle(reg,num);
        for(size_t i=0;i<num;++i)
        {
            h.release(reg[i]);
            reg[i] = 0;
        }


    }
}

Y_UTEST(small_mt)
{
    concurrent::fake_lock access;

    small::quarry Q;
    small::blocks B(4096,512,Q);
    {
        small::mt_proxy<small::vein> v64(access, Q(64) );
        doMT(v64);
    }
    std::cerr << Q << std::endl;

    {
        small::mt_proxy<small::arena> a64(access,B[64]);
        doMT(a64);
    }
    std::cerr << Q << std::endl;

}
Y_UTEST_DONE()

