
#include "y/memory/small/quarry.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

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
    std::cerr << Q << std::endl;


    alea.shuffle(blk,num);
    for(size_t i=0;i<num;++i)
    {
        block &b = blk[i];
        Q.push(b.entry,b.shift);
    }

    std::cerr << Q << std::endl;
    if(false)
    {
        static const unsigned ll[] =
        {
            0,0,0,0,2,2,2,3,3,3,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,9,9,9,10,10,10,10,11,11,11,12,12,12,13,13,13,13,14,14,14,15,15,15,16,16,16,16,17,17,17,18,18,18,19,19,19,19
        };

        std::cerr << "#ll=" << sizeof(ll)/sizeof(ll[0]) << std::endl;
        ios::ocstream fp1("qlen.dat");
        ios::ocstream fp2("qsup.dat");
        size_t p = 0;
        for(size_t i=0;i<small::vein::min_exp2;++i)
        {
            std::cerr << "0,";
        }
        for(size_t i=small::vein::min_exp2;i<=small::vein::max_exp2;++i)
        {
            char buffer[1024] = { 0 };
            memset(buffer,0,sizeof(buffer));
            snprintf(buffer, sizeof(buffer)-1, "%lu", (unsigned long) Q[i].block_size );
            const size_t n = strlen(buffer);
            //std::cerr << i << "->" << n << " (" << buffer << ")" << std::endl;
            fp1("%u %u\n", unsigned(i), unsigned(n) );
            Y_ASSERT(n==ll[i]);
            if(n>p)
            {
                fp2("%u %u\n", unsigned(i), unsigned(n) );
                p=n;
            }
            std::cerr << n;
            if(i<small::vein::max_exp2) std::cerr << ",";
        }
        std::cerr << std::endl;
    }


}
Y_UTEST_DONE()
