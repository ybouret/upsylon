#include "y/mkl/fit/share/polynomial.hpp"
#include "y/mkl/fit/least-squares.hpp"
#include <typeinfo>
#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;
using namespace Fit;

namespace {


    template <typename T>
    void doPoly()
    {
        const string  tid = typeid(T).name();
        Polynomial<T> P(3,"a");

        std::cerr << "names=" << P.names << std::endl;


        const size_t n = 10 + alea.leq(30);
        typename Type<T>::Series X = new vector<T>(n);
        typename Type<T>::Series Y = new vector<T>(n);
        typename Type<T>::Series Z = new vector<T>(n);

        for(size_t i=1;i<=n;++i)
        {
            X[i] = T(2.5) * alea.to<T>();
            Y[i] = cos_of(X[i]) + T(0.01) * alea.symm<T>();
            Z[i] = 0;
        }

        Sample<T>  sample(X,Y,Z);
        Variables &vars = sample.variables;
        P.appendTo(vars);

        sample.ready();


        vector<T> aorg( vars.sweep(), 0   );
        bVector   used( aorg.size(),  false );
        vector<T> aerr( aorg.size(),  0 );

        LeastSquares<T>            ls;
        typename Type<T>::Function F( &P, & Polynomial<T>::compute );

        for(size_t i=1;i<=used.size();++i)
        {
            std::cerr << std::endl;
            quark::ld(aorg,0);
            used[i] = true;
            std::cerr << "used=" << used << std::endl;

            Y_CHECK(P.initialize(sample, aorg, used, vars));
            vars.display(std::cerr,aorg);
            Y_CHECK( ls.fit(sample, F, aorg, used, aerr) );
            vars.display(std::cerr,aorg,aerr);
            {
                const string  fn = "adj_poly_" + tid + Common::Suffix(i-1) + ".dat";
                ios::ocstream fp(fn);
                sample.save(fp,true);
            }
            std::cerr << "\tR2=" << sample.computeR2() << std::endl;
        }

    }
}


Y_UTEST(fit_poly)
{

    doPoly<float>();
    doPoly<double>();

}
Y_UTEST_DONE()



