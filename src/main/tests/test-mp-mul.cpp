#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(mp_mul)
{
    
    std::cerr << "bits=";
    for(size_t bits=0;bits<=256;++bits)
    {
        std::cerr << bits  << '/';
        for(size_t iter=0;iter<8;++iter)
        {
            const mpn a = mpn(bits,alea);
            for(size_t i=0;i<256;++i)
            {
                const uint8_t x = uint8_t(i);
                const mpn     b = mpn::short_mul(a,x);
                const mpn     X = i;
                const mpn     p = a*X;
                Y_ASSERT(p==b);
            }
        }
    }
    std::cerr << std::endl;
}
Y_UTEST_DONE()

