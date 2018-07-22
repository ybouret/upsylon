
#include "y/math/kernel/tao.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/rt-clock.hpp"
#include "support.hpp"
#include <typeinfo>
using namespace upsylon;
using namespace math;

namespace
{
    template <typename T,typename U,typename V> static inline
    double do_mmul(const size_t n, const size_t m, concurrent::for_each *loop)
    {
        std::cerr << "mmul " << n  << "x" << m << " : ";
        rt_clock  clk;
        vector<T> a(n);
        matrix<U> M(n,m);
        vector<T> b(m);
        uint64_t tmx = 0;
        for(size_t iter=0;iter<32;++iter)
        {
            const uint64_t mark = rt_clock::ticks();
            tao::mul(a,M,b,loop);
            tmx += rt_clock::ticks() - mark;
        }
        std::cerr << clk(tmx) << std::endl;
        return clk(tmx);
    }

}

#define LOOP(CODE) do { \
const double tseq = CODE(r,c,NULL);  \
const double tpar = CODE(r,c,&loop); \
std::cerr << "\tSpeedUp: " << tseq/tpar << std::endl;\
} while(false)

Y_UTEST(tao2)
{
    concurrent::simd loop;

    for(size_t r=1;r<=100;r+=1+alea.leq(10))
    {
        for(size_t c=1;c<=100;c+=1+alea.leq(10))
        {
            LOOP( (do_mmul<float,float,float>) );
        }
    }
}
Y_UTEST_DONE()

