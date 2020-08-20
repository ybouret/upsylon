
#include "y/yap/rational.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/type/utils.hpp"
#include "y/sort/heap.hpp"

using namespace upsylon;
using namespace yap;

#define ITER 128

namespace {

    static inline void test_setup()
    {
        std::cerr << "---> test setup" << std::endl;
        vector<rational> Q;
        {
            const int      n = -10;
            const rational Q = n;
            Y_ASSERT(Q==n);
            Y_ASSERT(n==Q);
            Y_ASSERT(!(Q!=n));
            Y_ASSERT(!(n!=Q));
        }

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
                    Y_ASSERT(q==Q[i]);
                    Y_ASSERT(!(q!=Q[i]));
                }
                Y_ASSERT(total==written);
            }
        }
    }

    static inline void test_cmp()
    {
        std::cerr << "---> test cmp" << std::endl;

        const int      num[] = { -1, -1, 0,   1, 1 };
        const unsigned den[] = { 2,   3, 100, 5, 4 };


        vector<rational> Q;
        for(size_t i=0;i<sizeof(num)/sizeof(num[0]);++i)
        {
            const rational q(num[i],den[i]);
            Q.push_back(q);
        }
        std::cerr << "Q=" << Q << std::endl;
        const size_t size = Q.size();

        const int     lower = -1;
        const int     upper =  1;
        const integer Lower = lower;
        const integer Upper = upper;
        

        for(size_t i=1;i<size;++i)
        {
            const rational &lhs = Q[i];
            //std::cerr << "lhs=" << lhs << std::endl;
            Y_ASSERT(lhs==lhs);
            Y_ASSERT(!(lhs!=lhs));
            Y_ASSERT(lower<lhs);
            Y_ASSERT(lower<=lhs);
            Y_ASSERT(Lower<lhs);
            Y_ASSERT(Lower<=lhs);

            Y_ASSERT(lhs<upper);
            Y_ASSERT(lhs<Upper);
            Y_ASSERT(lhs<=upper);
            Y_ASSERT(lhs<=Upper);
            for(size_t j=i+1;j<=size;++j)
            {
                const rational &rhs = Q[j];
                //std::cerr << "\trhs=" << rhs << std::endl;
                Y_ASSERT(lhs<rhs);
                Y_ASSERT(lhs<=rhs);
                Y_ASSERT(rhs>=lhs);
                Y_ASSERT(rhs>lhs);
            }
        }

        alea.shuffle(*Q,Q.size());
        hsort(Q,rational::compare);

    }

}

Y_UTEST(yap_q)
{
    test_setup();
    test_cmp();
    std::cerr << "---> Memory Usage:" << std::endl;
    std::cerr << *natural::instance() << std::endl;
}
Y_UTEST_DONE()
