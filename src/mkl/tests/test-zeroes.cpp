#include "y/mkl/solve/zdriver.hpp"
#include "y/mkl/solve/zbis.hpp"
#include "y/mkl/solve/zrid.hpp"
#include "y/type/spec.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;

namespace {


    template <typename T>
    class dummy
    {
    public:
        size_t calls;

        inline  dummy() throw() : calls(0) {}
        inline ~dummy() throw() {}

        inline T operator()( const T x )
        {
            ++calls;
            return cos_of(x)*exp_of(-x);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };

    template <typename T, template <class> class ALGO>
    static inline void doZero()
    {
        zdriver<T,ALGO> solver;
        std::cerr << "[" << solver.method() << "]/" << type_name_of<T>() << std::endl;
        dummy<T>     F;
        const T      x0 = solver(F,0,2);
        std::cerr << "x0="    << x0      << std::endl;
        std::cerr << "F0="    << F(x0)   << std::endl;
        std::cerr << "\tcalls=" << F.calls << std::endl;


        const T xh = solver(T(0.5),F,0,T(1.5));
        std::cerr << "xh="    << xh      << std::endl;
        std::cerr << "Fh="    << F(xh)   << std::endl;
        std::cerr << "\tcalls=" << F.calls << std::endl;
        std::cerr << std::endl;
    }

    template < template <class> class ALGO>
    static inline void doZeros()
    {
        doZero<double,ALGO>();
        doZero<float,ALGO>();
    }
}


Y_UTEST(zeroes)
{
    if(argc>1)
    {
        for(int i=1;i<argc;++i)
        {
            const string arg = argv[i];
            if(arg=="bis") doZeros<zbis>();
            if(arg=="rid") doZeros<zrid>();

        }
    }
    else
    {
        doZeros<zbis>();
        doZeros<zrid>();
    }

}
Y_UTEST_DONE()

