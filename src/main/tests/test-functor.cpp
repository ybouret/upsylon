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
            std::cerr << "init@" << value << std::endl;
        }

        dummy(const dummy &d) : value(d.value)
        {
            std::cerr << "copy@value" << std::endl;
        }

        inline
        double operator()(void)
        {
            return value;
        }

        inline
        double operator()(double x)
        {
            return value+x;
        }

        double get0() throw() { return value; }

        static double Get0() throw() { return 0.0; }

        float get1( double x ) { return float(value+x); }

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

    template <typename FUNCTOR>
    static inline
    void do_test(FUNCTOR &F, double x)
    {
        std::cerr << "F=" << F(x) << std::endl;
        FUNCTOR G = F;
        std::cerr << "G=" << G(x) << std::endl;
    }

}

Y_UTEST(functor)
{
    dummy d(5);
    {
        std::cerr << "-- No Args" << std::endl;
        std::cerr << "using functor command" << std::endl;
        functor<double,null_type> f0( &d, & dummy::get0 );
        do_test(f0);
        std::cerr << "using functor callback" << std::endl;
        functor<double,null_type> f1( d );
        d.value++;
        do_test(f1);
        std::cerr << "recall functor command" << std::endl;
        do_test(f0);
        std::cerr << "using functor callback/C-style" << std::endl;
        double (*proc)() = dummy::Get0;
        functor<double,null_type> f2( proc );
        do_test(f2);
    }

    {
        std::cerr << "using functor command" << std::endl;
        functor<double,TL1(double)> f0( &d, & dummy::get1 );
        do_test(f0,7.13);
        std::cerr << "using functor callback" << std::endl;
        functor<double,TL1(double)> f1(d);
        d.value++;
        do_test(f1,7.13);
        std::cerr << "recall functor command" << std::endl;
        do_test(f0,7.13);
    }

}
Y_UTEST_DONE()

