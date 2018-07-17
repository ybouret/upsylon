#include "y/math/kernel/tao.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/os/rt-clock.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    static const size_t nmin = 16;
    static const size_t nmax = 16384;



    template <typename T>
    void do_test1(concurrent::for_each &loop)
    {
        // ld
        rt_clock clk;
        
        uint64_t nseq=0;
        uint64_t npar=0;
        for(size_t n=nmin;n<=nmax;n*=2)
        {
            vector<T> a(n);
            for(size_t iter=0;iter<128;++iter)
            {
                const T   v = support::get<T>();

                {
                    const uint64_t mark = rt_clock::ticks();
                    tao::_ld(a,v);
                    nseq +=rt_clock::ticks()-mark;
                }

                {
                    const uint64_t mark = rt_clock::ticks();
                    tao::_ld(a,v,loop);
                    npar += rt_clock::ticks()-mark;
                }
            }
        }
        std::cerr << "nseq=" << clk(nseq) << ", npar=" << clk(npar) << std::endl;
    }
}

Y_UTEST(tao)
{
    concurrent::simd loop;

    do_test1<float>(loop);
    do_test1<double>(loop);
    do_test1<int>(loop);
    //do_test1<mpn>(loop);

}
Y_UTEST_DONE()

