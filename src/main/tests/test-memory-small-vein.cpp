
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

    for(size_t i=small::vein::min_shift;i<=14;++i)
    {
        small::vein S(i);
        std::cerr << "vein: " << S.bytes << std::endl;

        size_t n=0;
        while(n<num)
        {
            reg[n] = S.query();
            Y_ASSERT(reg[n]);
            ++n;
        }
        assert(n==num);
        alea.shuffle(reg,num);
        while(n>num/2)
        {
            --n;
            Y_ASSERT(NULL!=reg[n]);
            S.store(reg[n]);
        }

        while(n<num)
        {
            reg[n] = S.query();
            Y_ASSERT(reg[n]);
            ++n;
        }

        alea.shuffle(reg,num);

        while(n>0)
        {
            --n;
            Y_ASSERT(NULL!=reg[n]);
            S.store(reg[n]);
        }
        std::cerr << "|_has " << S.cache.size * S.bytes << " bytes" << std::endl;

    }
    std::cerr << "small::vein::min_shift=" << small::vein::min_shift << std::endl;
    std::cerr << "small::vein::max_shift=" << small::vein::max_shift << std::endl;

    
}
Y_UTEST_DONE()

