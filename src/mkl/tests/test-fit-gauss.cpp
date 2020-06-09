
#include "y/math/fit/share/gaussian.hpp"
#include "y/math/fit/least-squares.hpp"
#include "y/ios/ocstream.hpp"

#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;
using namespace Fit;


namespace {

    template <typename T>
    void doGauss()
    {
        const string  tid = typeid(T).name();
        Gauss<T> G(2,NULL);

        std::cerr << "names=" << G.names << std::endl;

        const size_t n = 50 + alea.leq(50);
        typename Type<T>::Series X = new vector<T>(n);
        typename Type<T>::Series Y = new vector<T>(n);
        typename Type<T>::Series Z = new vector<T>(n);

        Sample<T>  sample(X,Y,Z);
        Variables &vars = sample.variables;
        G.appendTo(vars);


        vector<T> aorg( vars.sweep(), 0   );
        bVector   used( aorg.size(),  true );
        vector<T> aerr( aorg.size(),  0 );

        vars(aorg,"A1")   = T(1.2);
        vars(aorg,"mu1")  = T(2.3);
        vars(aorg,"sig1") = T(0.7);

        vars(aorg,"A2")   = T(2.7);
        vars(aorg,"mu2")  = T(5.2);
        vars(aorg,"sig2") = T(0.55);


        for(size_t i=1;i<=n;++i)
        {
            X[i] = T(10.0) * alea.to<T>();
            Y[i] = G.compute(X[i], aorg, vars) + T(0.05) * alea.symm<T>();
            Z[i] = 0;
        }



        sample.ready();



        LeastSquares<T>            ls(true);
        typename Type<T>::Function F( &G, & Gauss<T>::compute );

        Y_CHECK(ls.fit(sample, F, aorg, used, aerr));
        vars.display(std::cerr,aorg,aerr);
        
        {
            const string  fn = "adj_gauss_" + tid + ".dat";
            ios::ocstream fp(fn);
            sample.save(fp,true);
        }
        {
            const string fn = "fcn_gauss_" + tid + ".dat";
            ios::ocstream fp(fn);
            for(T x=0;x<=10;x+=T(0.01))
            {
                fp("%g %g\n", double(x), double( G.compute(x,aorg,vars) ));
            }
        }
        std::cerr << "\tR2=" << sample.computeR2() << std::endl;

    }

}



Y_UTEST(fit_gauss)
{
    doGauss<float>();
}
Y_UTEST_DONE()

