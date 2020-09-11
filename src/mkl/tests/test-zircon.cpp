

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
        T Ca;
        T Ka;
        void compute( addressable<T> &F, const accessible<T> &X )
        {
            const T Kw = 1e-14;
            const T h  = X[1];
            const T w  = X[2];
            const T AH = X[3];
            const T Am = X[4];

            F[1] = h*w   - Kw;
            F[2] = AH*Ka - h*Am;
            F[3] = AH+Am - Ca;
            F[4] = h - w - Am;
        }

        void jacobian( matrix<T> &J, const accessible<T> &X)
        {
            const T h  = X[1];
            const T w  = X[2];
            //const T AH = X[3];
            const T Am = X[4];
            J.ld(0);
            {
                array<T> &dF1 = J[1];
                dF1[1] = w; dF1[2] = h;
            }


            {
                array<T> &dF2 = J[2];
                dF2[1] = -Am; dF2[3] = Ka; dF2[4] = -h;
            }

            {
                array<T> &dF3 = J[3];
                dF3[3] = 1;
                dF3[4] = 1;
            }

            {
                array<T> &dF4 = J[4];
                dF4[1] = 1; dF4[2]= -1; dF4[4] = -1;
            }

        }
    };

}

#include "y/string/convert.hpp"

Y_UTEST(zircon)
{
    
    //concurrent::singleton::verbose = true;

    mysys<double>  sys = { 0.00, pow(10.0,-4.8) };

    if(argc>1)
    {
        sys.Ca = string_convert::to<double>( argv[1], "Ca" );
    }

    if(false)
    {
        matrix<double> J(2,2);
        J[1][1] = 0; J[1][2] = 0;
        J[2][1] = 1; J[2][2] = -1;
        doZNL(J);

        const double r = alea.symm<double>();
        J[1][1] = r; J[1][2] = r;
        doZNL(J);
    }


    zircon<double> zrc;
    zrc.verbose  = true;
    
    numeric<double>::vector_field f(&sys, &mysys<double>::compute);
    numeric<double>::jacobian     fjac(&sys,&mysys<double>::jacobian);

    vector<double> F(4,0);
    vector<double> X(4,0);

    f(F,X);
    zrc.cycle(F,X,f,fjac);

    X[1] = 1e-7 * alea.to<double>();
    f(F,X);
    zrc.cycle(F,X,f,fjac);

    //exit(1);
    
}
Y_UTEST_DONE()
