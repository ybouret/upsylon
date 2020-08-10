
#include "y/memory/tight/stock.hpp"
#include "y/memory/tight/quarry.hpp"
#include "y/memory/tight/blocks.hpp"

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

        small::stock s(h);
        s.reserve(300);
        std::cerr << "available: " << s.available() << std::endl;
        for(size_t i=0;i<num;++i)
        {
            reg[i] = s.query();
        }

        std::cerr << "available: " << s.available() << std::endl;

        alea.shuffle(reg,num);
        for(size_t i=0;i<num;++i)
        {
            s.store(reg[i]);
            reg[i] = 0;
        }
        std::cerr << "available: " << s.available() << std::endl;

    }
}

Y_UTEST(small_mt)
{
    std::cerr << "stock::min_block_size=" << small::stock::min_block_size << std::endl;
    concurrent::fake_lock access;

    small::quarry Q;
    small::blocks B(4096,512,Q);
    std::cerr << "Minimal Quarry: " << std::endl;
    std::cerr << Q << std::endl;

    {
        small::mt<small::vein> v64(access, Q(64) );
        doMT(v64);
    }
    std::cerr << Q << std::endl;

    {
        small::mt<small::arena> a64(access,B[64]);
        doMT(a64);
    }
    std::cerr << Q << std::endl;

}
Y_UTEST_DONE()

