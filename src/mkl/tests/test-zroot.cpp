#include "y/mkl/root/bisection.hpp"
#include "y/mkl/root/secant.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;
namespace {

    struct Call
    {
        unsigned calls;
        double operator()(double x)
        {
            ++calls;
            return 0.1 + cos(x*x*0.9);
        }
    };

    template <typename PROC>
    void doTestZ( PROC &proc, const char *which )
    {
        std::cerr << "<" << which << ">" << std::endl;
        Call F;
        triplet<double>   x = {0,0,2};
        triplet<double>   f = {F(x.a),-1,F(x.c)};
        F.calls = 0;
        Y_CHECK(proc(F,x,f));
        std::cerr << "x=" << x.b << std::endl;
        std::cerr << "|_calls=" << F.calls << std::endl;

        F.calls = 0;
        std::cerr << "x=" << proc.run(proc,F,2,2.5) << std::endl;
        std::cerr << "|_calls=" << F.calls << std::endl;


    }
}
Y_UTEST(zroot)
{
    {
        bisection<double> bis;
        doTestZ(bis,"bisection");
    }

    {
        secant<double> sec;
        doTestZ(sec,"secant");
    }



}
Y_UTEST_DONE()

