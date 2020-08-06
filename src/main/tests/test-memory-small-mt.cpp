
#include "y/memory/small/hoard.hpp"
#include "y/memory/small/quarry.hpp"
#include "y/memory/small/blocks.hpp"

#include "y/concurrent/fake-lock.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace memory;

namespace
{
    static inline void doMT( small::hoard &h )
    {
        std::cerr << "block_size=" << h.block_size << std::endl;
    }
}

Y_UTEST(small_mt)
{
    concurrent::fake_lock access;

    small::quarry Q;
    small::blocks B(4096,512,Q);
    {
        small::mt<small::vein> v64(access, Q(64) );
        doMT(v64);
    }

    {
        small::mt<small::arena> a64(access,B[64]);
        doMT(a64);
    }

}
Y_UTEST_DONE()

