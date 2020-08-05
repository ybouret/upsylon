
#include "y/memory/small/quarry.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"

#include <iomanip>

using namespace upsylon;
using namespace memory;

namespace {

    struct block
    {
        void  *entry;
        size_t shift;
    };

}

Y_UTEST(small_quarry)
{
    small::quarry Q;

    block        blk[ 1024 ];
    const size_t num = sizeof(blk)/sizeof(blk[0]);

    for(size_t i=0;i<num;++i)
    {
        block &b = blk[i];
        b.shift  = alea.leq(16);
        b.entry  = Q.pull(b.shift);
    }
    alea.shuffle(blk,num);
    for(size_t i=0;i<num;++i)
    {
        block &b = blk[i];
        Q.push(b.entry,b.shift);
    }

}
Y_UTEST_DONE()
