
#include "y/yap/library.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"
#include <cmath>
#include <cstdio>

using namespace upsylon;
using namespace yap;

namespace {

}

Y_UTEST(yap_log)
{
    {
        ios::ocstream fp("ilog.dat");
        for(unsigned i=1;i<=100;++i)
        {
            const double x = double(i);
            const double l = log(x);
            const double v = floor(l+0.5);
            const double f = floor(l);
            const double c = ceil(l);
            fp("%g %g %g %g %g\n", x,l,v,f,c);

        }
    }

    for(unsigned bits=1;bits<=100;++bits)
    {
        for(unsigned iter=0;iter<4;++iter)
        {
            const natural n(alea,bits); Y_ASSERT(bits==n.bits());
            std::cerr << "log(" << n <<  ") = " << natural::log_of(n) << std::endl;
        }
    }


}
Y_UTEST_DONE()

