#include "y/tensor/tensor3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;


namespace {

    template <typename T>
    static inline void doTest()
    {
        tensor1d<T> t1( 1+alea.leq(10) );
        support::fill1D(t1);
        std::cerr << "tensor1d.bytes=" << t1.allocated() << std::endl;
        
        tensor2d<T> t2( 1+alea.leq(10), 1+alea.leq(10) );
        support::fill2D(t2);
        std::cerr << "tensor2d.bytes=" << t2.allocated() << std::endl;

        
        tensor3d<T> t3( 1+alea.leq(10), 1+alea.leq(10), 1+alea.leq(10) );
        support::fill3D(t3);
        std::cerr << "tensor3d.bytes=" << t3.allocated() << std::endl;

    }
}

Y_UTEST(tensors)
{
    doTest<float>();
    doTest<int>();
    doTest<double>();
    doTest<string>();

}
Y_UTEST_DONE()

