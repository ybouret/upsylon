

#include "y/utest/run.hpp"
#include "y/mkl/root/znl.hpp"
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

}


Y_UTEST(znl)
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


}
Y_UTEST_DONE()
