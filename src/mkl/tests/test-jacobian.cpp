#include "y/utest/run.hpp"
#include "y/mkl/fcn/djacobian.hpp"
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
        int x = 1;
        std::cerr << "x=" << x << std::endl;
        {
            core::temporary_value<int> temp(x,2);
            std::cerr << "x=" << x << std::endl;
        }
        std::cerr << "x=" << x << std::endl;

    }


    {
        typedef void (*vfield)( addressable<double> &, const accessible<double> &);
        vfield vf = F1<double>;
        mkl::numeric<double>::vector_field F(vf);
        mkl::derivative<double>::pointer   D = new mkl::derivative<double>();
        vector<double> X(2,0);
        X[1] = 1;
        X[2] = 2;
        matrix<double> J(1,2);
        mkl::djacobian<double> Jacobian(F,D);
        
        Jacobian(J,X);
        std::cerr << "X=" << X << std::endl;
        std::cerr << "J=" << J << std::endl;
    }

    {
        typedef void (*vfield)( addressable<float> &, const accessible<float> &);
        vfield vf = F2<float>;
        mkl::numeric<float>::vector_field F(vf);
        mkl::derivative<float>::pointer   D = new mkl::derivative<float>();
        vector<float> X(3,0);
        X[1] = 1;
        X[2] = 1;
        X[3] = 1;
        matrix<float> J(2,3);
        mkl::djacobian<float> Jacobian(F,D);

        Jacobian(J,X);
        std::cerr << "X=" << X << std::endl;
        std::cerr << "J=" << J << std::endl;
    }

}
Y_UTEST_DONE()


