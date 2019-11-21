#include "y/core/zero-flux-index.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;

Y_UTEST(zfi)
{
    typedef core::zero_flux_index<0> zfi0;
    typedef core::zero_flux_index<1> zfi1;

    zfi0 I0(8);
    zfi1 I1(8);

    ios::ocstream fp("zfi.dat");
    for(unit_t i=-16;i<=16;++i)
    {
        std::cerr << i << " -> ";
        const unit_t i0 = I0(i);
        std::cerr << i0;

        const unit_t i1 = I1(i);
        std::cerr << " : " << i1;
        

        fp("%ld %ld %ld\n", long(i), long( i0 ), long(i1) );
        std::cerr << std::endl;
    }



}
Y_UTEST_DONE()

