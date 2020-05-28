#include "y/tensor/tensor3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;


namespace {

    template <typename T>
    static inline void doTest()
    {
        std::cerr << std::endl;
        const string &id = type_name_of<T>();
        
        tensor1d<T> t1( 1+alea.leq(10) );
        support::fill1D(t1);
        std::cerr << "tensor1d<" << id << ">.bytes=" << t1.allocated() << std::endl;
        
        tensor2d<T> t2( 1+alea.leq(10), 1+alea.leq(10) );
        support::fill2D(t2);
        std::cerr << "tensor2d<" << id << ">.bytes=" << t2.allocated() << std::endl;

        
        tensor3d<T> t3( 1+alea.leq(10), 1+alea.leq(10), 1+alea.leq(10) );
        support::fill3D(t3);
        std::cerr << "tensor3d<" << id << ">.bytes=" << t3.allocated() << std::endl;

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

