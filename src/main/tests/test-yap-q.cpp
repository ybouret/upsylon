
#include "y/yap/rational.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/type/utils.hpp"


using namespace upsylon;
using namespace yap;

#define ITER 128

namespace {

    static inline void test_setup()
    {
        std::cerr << "---> test setup" << std::endl;
        vector<rational> Q;

        for(size_t i=0;i<ITER;++i)
        {
            {
                const rational q( alea.full<number::itype>() );
                Q.push_back(q);
            }

            {
                const rational q( alea.full<number::itype>(), max_of<number::utype>(1,alea.full<number::utype>()) );
                Q.push_back(q);
            }

            {
                const integer  n(alea,alea.leq(100));
                const natural  d(alea,1+alea.leq(100));
                const rational q(n,d);
                Q.push_back(q);
            }
        }

        const size_t nq = Q.size();
        {
            size_t written = 0;
            {
                ios::ocstream fp("apq.dat");
                for(size_t i=1;i<=nq;++i)
                {
                    written  += Q[i].serialize(fp);
                }
            }
            std::cerr << "written=" << written << std::endl;
            {
                ios::icstream fp("apq.dat");
                size_t total = 0;
                for(size_t i=1;i<=nq;++i)
                {
                    size_t nr = 0;
                    const rational q = rational::read(fp, nr, "q");
                    total += nr;

                }
                Y_ASSERT(total==written);
            }
        }

    }

}

Y_UTEST(yap_q)
{
    test_setup();

    std::cerr << std::endl;
    std::cerr << "Memory Usage:" << std::endl;
    std::cerr << *natural::instance() << std::endl;
}
Y_UTEST_DONE()
