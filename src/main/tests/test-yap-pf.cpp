
#include "y/yap/prime/factors.hpp"
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

Y_UTEST(yap_pf)
{


    prime_iterator       p( library::instance() );
    vector<prime_factor> pf;
    {
        size_t written = 0;
        {
            ios::ocstream fp("appf.dat");
            for(size_t i=10+alea.leq(100);i>0;--i,++p)
            {
                prime_factor   f(*p,1+alea.leq(5));
                written += f.serialize(fp);
                pf << f;
            }
        }
        std::cerr << "written=" << written << " for " << pf.size() << " factors" << std::endl;
        {
            ios::icstream fp("appf.dat");
            size_t nread = 0;
            for(size_t i=1;i<=pf.size();++i)
            {
                size_t             d = 0;
                const prime_factor f = prime_factor::read(fp,d,"someone");
                Y_ASSERT(f==pf[i]);
                Y_ASSERT(!(f!=pf[i]));
                nread += d;
            }
            std::cerr << "nread=" << nread << std::endl;
            Y_ASSERT(nread==written);
        }
    }
    for(size_t i=1;i<=pf.size();++i)
    {
        std::cerr << pf[i] << " = " << pf[i].value() << std::endl;
    }
    Y_UTEST_SIZEOF(natural);
    Y_UTEST_SIZEOF(prime);
    Y_UTEST_SIZEOF(prime_factor);
    Y_UTEST_SIZEOF(prime_factors);

    if(argc>1)
    {
        const natural n = natural::parse( argv[1] );
        (std::cerr << "n=" << n << "=" ).flush();
        const prime_factors F = n;
        std::cerr << F << std::endl;
        const natural p = F.value();
        Y_ASSERT(n==p);
        {
            const size_t written = F.save_to("apfd.dat");
            std::cerr << "written=" << written << std::endl;
            {
                ios::icstream fp("apfd.dat");

            }
        }

        if(argc>2)
        {
            const natural       m = natural::parse( argv[2] );
            (std::cerr << "m=" << m << "=" ).flush();
            const prime_factors G = m;
            std::cerr << G << std::endl;

            const prime_factors P = prime_factors::mul(F,G);
            std::cerr << "P=" << P << std::endl;
            const natural p = m*n;
            Y_ASSERT(P.value()==p);
        }

    }

    std::cerr << "factorial decomposition:" << std::endl;
    {
        prime_factors P(1);
        for(size_t n=2;n<=30;++n)
        {
            const prime_factors tmp(n);
            prime_factors prod = prime_factors::mul(tmp,P);
            P.xch(prod);
            std::cerr << n << "! = " << P << std::endl;
        }
    }

}
Y_UTEST_DONE()

