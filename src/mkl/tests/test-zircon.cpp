

#include "y/utest/run.hpp"
#include "y/mkl/root/zircon.hpp"
#include "y/mkl/fcn/djacobian.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/sequence.hpp"
#include "y/sparse/matrix.hpp"

using namespace upsylon;
using namespace mkl;


namespace {

    


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

    template <typename T>
    struct inter
    {
        T a;
        void compute( addressable<T> &F, const accessible<T> &X )
        {
            F[1] = X[1]*X[1] + X[2]*X[2] - 1.0;
            F[2] = X[2]-(X[1]+a);
        }

        void jacobian(matrix<T> &J, const accessible<T> &X)
        {
            J[1][1] = 2*X[1]; J[1][2] = 2*X[2];
            J[2][1] = -1;     J[2][2] = 1;
        }
    };
}

#include "y/string/convert.hpp"
#include "y/ios/ocstream.hpp"

Y_UTEST(zircon)
{
    zircon<double> zrc;
    zrc.verbose  = true;

    if(false)
    {
        mysys<double>  sys = { 0.00, pow(10.0,-4.8) };

        if(argc>1)
        {
            sys.Ca = string_convert::to<double>( argv[1], "Ca" );
        }

        




        numeric<double>::vector_field f(&sys, &mysys<double>::compute);
        numeric<double>::jacobian     fjac(&sys,&mysys<double>::jacobian);

        vector<double> F(4,0);
        vector<double> X(4,0);

        f(F,X);

        zrc.cycle2(F, X, f, fjac);

        return 0;

        zrc.cycle(F,X,f,fjac);

        X[1] = 1e-7 * alea.to<double>();
        f(F,X);
        zrc.cycle(F,X,f,fjac);

    }

    if(true)
    {
        inter<double> Inter = { 0.1 };
        if(argc>1)
        {
            Inter.a = string_convert::to<double>( argv[1], "a" );
        }
        
        numeric<double>::vector_field f(&Inter, &inter<double>::compute);
        numeric<double>::jacobian     fjac(&Inter,&inter<double>::jacobian);

        vector<double> F(2,0);
        vector<double> X(2,0);

        X[1] = alea.symm<double>();
        X[2] = alea.symm<double>();
        const string fn = "inter.dat";

        //X[1]=1; X[2]=-1;

        X[1] = 0; X[2] = 0;

        f(F,X);

        ios::ocstream::overwrite(fn);
        ios::ocstream::echo(fn,"%g %g 0 %g\n",X[1],X[2], quark::mod2<double>::of(F)/2);

        int cycle = 0;

        while( !zrc.cycle2(F,X,f,fjac) )
        {
            ++cycle;
            ios::ocstream::echo(fn,"%g %g %d %g\n",X[1],X[2], cycle, quark::mod2<double>::of(F)/2);
            if(cycle>32) break;
        }

    }

    
}
Y_UTEST_DONE()
