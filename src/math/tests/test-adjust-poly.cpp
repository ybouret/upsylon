#include "y/math/adjust/share/polynomial.hpp"
#include "y/utest/run.hpp"
#include "y/math/adjust/least-squares.hpp"
#include <typeinfo>
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;
using namespace Adjust;

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
        {
            const string  fn = "adj_poly_" + tid + ".dat";
            ios::ocstream fp(fn);
            sample.save(fp,true);
        }

    }
}


Y_UTEST(adjust_poly)
{

    doPoly<float>();
    doPoly<double>();

}
Y_UTEST_DONE()



