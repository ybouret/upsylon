
#include "y/yap/prime/sprp.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/string.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"
#include <cmath>
#include <cstdio>

using namespace upsylon;
using namespace yap;

Y_UTEST(yap_sprp)
{
    Y_CHECK( sprp(2047).base(2) );
    Y_CHECK( sprp( 121).base(3)  );
    Y_CHECK( sprp( 781).base(5)  );
    Y_CHECK( sprp( 25) .base(7)   );

    for(size_t bits=1;bits<=100;bits += 1+alea.leq(10) )
    {
        const natural n(alea,bits);
        const natural b = sprp::end(n);
        std::cerr << "Miller's end(" << n << ") = " << b << std::endl;

    }

    if(argc>1)
    {
        const natural x = natural::parse(argv[1]);
        sprp s(x);
        std::cerr << s.n << " = " << "2^" << s.s << "*" << *s.l.head << "+1" << std::endl;

        for(int i=2;i<argc;++i)
        {
            const natural a = natural::parse(argv[i]);
            std::cerr << a << "-SPRP(" << x << ") : " << s.base(a) << std::endl;
        }
    }

    
}
Y_UTEST_DONE()

