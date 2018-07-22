#include "y/functor.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
namespace
{
    class dummy
    {
    public:
        int value;

        dummy(const int v) : value(v)
        {
        }

        dummy(const dummy &d) : value(d.value)
        {
        }

        double get0() throw() { return value; }

        static double Get0() throw() { return 0.0; }

        float get1( double x ) { return value+x; }

        static double Get1(double x) throw() { return 1.0+x; }

    private:
        Y_DISABLE_ASSIGN(dummy);
    };

    template <typename FUNCTOR>
    static inline
    void do_test(FUNCTOR &F)
    {
        std::cerr << "F=" << F() << std::endl;
        FUNCTOR G = F;
        std::cerr << "G=" << G() << std::endl;
    }

}

Y_UTEST(functor)
{
    dummy d(5);
    {
        functor<double,null_type> f0( &d, & dummy::get0 );
        std::cerr << "f0=" << f0() << std::endl;
        do_test(f0);
        double (*proc0)() = dummy::Get0;
        functor<double,null_type> F0 = proc0;
        std::cerr << "F0=" << F0() << std::endl;
        do_test(F0);
    }

    {
        functor<double,TL1(double)> f1( &d, & dummy::get1 );
    }

}
Y_UTEST_DONE()

