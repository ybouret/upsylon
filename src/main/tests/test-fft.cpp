#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline
    void do_test()
    {
        for(size_t i=0;i<sizeof(fft<T>::sin_table)/sizeof(fft<T>::sin_table[0]);++i)
        {
            std::cerr << "sin_table[" << i << "]=" <<fft<T>::sin_table[i] << std::endl;
        }

    }
}

Y_UTEST(fft)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()


