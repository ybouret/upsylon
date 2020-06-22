#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(mp_mul)
{
    
    for(size_t bits=0;bits<=256;++bits)
    {
        for(size_t iter=0;iter<8;++iter)
        {
            const mpn a = mpn(bits,alea);
            // std::cerr << "a=" << a << std::endl;
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
}
Y_UTEST_DONE()

