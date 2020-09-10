

#include "y/utest/run.hpp"
#include "y/mkl/root/zircon.hpp"
#include "y/mkl/fcn/djacobian.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/sequence.hpp"
#include "y/sparse/matrix.hpp"

using namespace upsylon;
using namespace mkl;


namespace {

    static inline void doZNL(const matrix<double> &J)
    {
        matrix<double> u = J;
        vector<double> w(2,0);
        matrix<double> v(2,2);

        Y_CHECK(svd::build(u,w,v));
        sparse_matrix<double> W(w);
        std::cerr << "J=" << J << std::endl;
        std::cerr << "u=" << u << std::endl;
        std::cerr << "w=" << W << std::endl;
        std::cerr << "v=" << v << std::endl;

        const size_t ker = __find<double>::truncate(w);
        std::cerr << "Ker=" << ker << std::endl;
        std::cerr << "w=" << w << std::endl;

    }

    template <typename T>
    struct mysys
    {
        T charge;
        void compute( addressable<T> &F, const accessible<T> &X )
        {
            F[1] = X[1] * X[2] - 1e-14;
            F[2] = X[1] - X[2] + charge;
        }

        void jacobian( matrix<T> &J, const accessible<T> &X)
        {
            J[1][1] = X[2]; J[1][2] = X[1];
            J[2][1] = 1;    J[2][1] = -1;
        }
    };

}


Y_UTEST(zircon)
{
    matrix<double> J(2,2);
    J[1][1] = 0; J[1][2] = 0;
    J[2][1] = 1; J[2][2] = -1;

    doZNL(J);
    {
        const double r = alea.symm<double>();
        J[1][1] = r; J[1][2] = r;
    }
    doZNL(J);

    zircon<double> zrc;
    zrc.verbose  = true;
    
    mysys<double>  sys = { 0.0 };
    numeric<double>::vector_field f(&sys, &mysys<double>::compute);
    jacobian<double>::type        fjac(&sys,&mysys<double>::jacobian);

    vector<double> F(2,0);
    vector<double> X(2,0);

    f(F,X);
    zrc.cycle(F,X,f,fjac);


}
Y_UTEST_DONE()
