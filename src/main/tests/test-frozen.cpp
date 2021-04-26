#include "y/container/frozen.hpp"
#include "support.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline
    void do_zf(const char *ID)
    {
        assert(ID);
        std::cerr << "const_field<" << ID << ">" << std::endl;
        frozen<T> zf;
        std::cerr << "|_bytes=" << zf.block_size << std::endl;
        std::cerr << "|_[" << *zf << "]" << std::endl;
    }

}

#define Y_ZF(TYPE) do_zf<TYPE>( #TYPE )

Y_UTEST(frozen)
{
    Y_ZF(int);
    Y_ZF(float);
    Y_ZF(double);
    Y_ZF(complex<float>);
    Y_ZF(complex<double>);
    Y_ZF(point2d<unit_t>);
    Y_ZF(string);
    Y_ZF(apn);
    Y_ZF(apz);
    Y_ZF(apq);

    {
        frozen<string> cfs( "hello" );
        std::cerr << "cfs=[" << *cfs << "]" << std::endl;
    }

    {
        frozen<apq> cfq(10);
        std::cerr << "cfq=[" << *cfq << "]" << std::endl;
    }
    
    {
        frozen<apq> cfq(10,20);
        std::cerr << "cfq=[" << *cfq << "]" << std::endl;
    }

}
Y_UTEST_DONE()

