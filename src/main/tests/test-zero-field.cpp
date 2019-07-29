#include "y/container/zero-field.hpp"
#include "support.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline
    void do_zf(const char *ID)
    {
        assert(ID);
        std::cerr << "zero_field<" << ID << ">" << std::endl;
        zero_field<T> zf;
        std::cerr << "|_bytes=" << zf.item_size << std::endl;
        std::cerr << "|_[" << zf.zero << "]" << std::endl;
    }

}

#define Y_ZF(TYPE) do_zf<TYPE>( #TYPE )

Y_UTEST(zero_field)
{
    Y_ZF(int);
    Y_ZF(float);
    Y_ZF(double);
    Y_ZF(complex<float>);
    Y_ZF(complex<double>);
    Y_ZF(point2d<unit_t>);
    Y_ZF(string);
    Y_ZF(mpn);
    Y_ZF(mpz);
    Y_ZF(mpq);

}
Y_UTEST_DONE()

