
#include "y/memory/small/vein.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"

#include <iomanip>

using namespace upsylon;
using namespace memory;

 

Y_UTEST(small_vein)
{

    void        *reg[ 256 ];
    const size_t num = sizeof(reg)/sizeof(reg[0]);

    memset(reg,0,sizeof(reg));
    for(size_t i=small::vein::min_shift;i<=14;++i)
    {
        small::vein S(i);
        std::cerr << "vein: " << S.block_size << std::endl;

        size_t n=0;
        while(n<num)
        {
            reg[n] = S.acquire();
            Y_ASSERT(reg[n]);
            ++n;
        }
        Y_ASSERT(n==num);
        alea.shuffle(reg,num);
        while(n>num/2)
        {
            --n;
            Y_ASSERT(NULL!=reg[n]);
            S.release(reg[n]);
            reg[n] = 0;
        }

        while(n<num)
        {
            reg[n] = S.acquire();
            Y_ASSERT(reg[n]);
            ++n;
        }
        Y_ASSERT(n==num);

        alea.shuffle(reg,num);

        while(n>0)
        {
            --n;
            Y_ASSERT(NULL!=reg[n]);
            S.release(reg[n]);
            reg[n] = 0;
        }
        Y_ASSERT(0==n);
        std::cerr << "|_has " << S.chest.size * S.block_size << " bytes" << std::endl;
    }
    
    std::cerr << "small::vein::min_shift=" << small::vein::min_shift << std::endl;
    std::cerr << "small::vein::max_shift=" << small::vein::max_shift << std::endl;

    
}
Y_UTEST_DONE()

