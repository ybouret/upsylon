#include "y/utest/run.hpp"
#include "y/mkl/fcn/jacobian.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{
    // [1:2]
    template <typename T> inline
    void F1( addressable<T> &V, const accessible<T> &X )
    {
        V[1] = 2*X[1] - 3*X[2];
    }
    
    // [2:3]
    template <typename T> inline
    void F2( addressable<T> &V, const accessible<T> &X )
    {
        V[1] = 2*X[1]    - 3*X[2] + 4 * X[3];
        V[2] = X[1]*X[1] - X[2]*X[2]*X[2] + X[3]*X[3]*X[3]*X[3];
    }
    
    
}

Y_UTEST(jacobian)
{
    {
        vector<double> X(2,0);
        X[1] = 1;
        X[2] = 2;
        matrix<double> J(1,2);
        mkl::jacobian<double> Jacobian;
        
        Jacobian(J,F1<double>,X);
        std::cerr << "X=" << X << std::endl;
        std::cerr << "J=" << J << std::endl;
    }
    
    {
        vector<float> X(3,0);
        X[1] = 1;
        X[2] = 1;
        X[3] = 1;
        matrix<float> J(2,3);
        mkl::jacobian<float> Jacobian;
        
        Jacobian(J,F2<float>,X);
        std::cerr << "X=" << X << std::endl;
        std::cerr << "J=" << J << std::endl;
    }
    
}
Y_UTEST_DONE()


