
#include "y/yap/prime/ratio.hpp"
#include "y/yap/prime/iterator.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace yap;


namespace {



}
#include "y/string/convert.hpp"

Y_UTEST(yap_pr)
{
    library &apl = library::instance();
    apl.prefetch(100);
    
    if(argc>1)
    {
        natural num = natural::parse(argv[1]);
        natural den = 1;
        if(argc>2)
        {
            den = natural::parse(argv[2]);
        }
        std::cerr << "num = " << num << std::endl;
        std::cerr << "den = " << den << std::endl;
        const prime_factors Fn = num;
        std::cerr << "Fn  = " << Fn << std::endl;
        const prime_factors Fd = den;
        std::cerr << "Fd  = " << Fd << std::endl;
        const prime_ratio Q(Fn,Fd);
        std::cerr << "Q   = " << Q.num << "/" << Q.den << " = " << Q << std::endl;
    }

    
    
   
}
Y_UTEST_DONE()

