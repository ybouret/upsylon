#include "y/mkl/signal/smooth.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace mkl;

Y_UTEST(smooth)
{

    size_t deg = 1;
    double dx  = 0.1;

    if(argc>1)
    {
        deg = string_convert::to<size_t>(argv[1],"deg");
    }

    if(argc>2)
    {
        dx = string_convert::to<double>(argv[2],"dx");
    }

    vector<double> X;
    vector<double> Y;

    {
        X << 0;
        Y << sin( X.back() );
        for(size_t n=10+alea.leq(100);n>0;--n)
        {
            const double x = alea.to<double>() * 1.5 * numeric<double>::pi;
            X << x;
            Y << sin(x) + 0.02 * alea.symm<double>();
        }
        hsort(X,Y, comparison::increasing<double> );

        ios::ocstream fp("smdata.dat");
        for(size_t i=1;i<=X.size();++i)
        {
            fp("%g %g\n", X[i], Y[i]);
        }
    }

    extend<double> data(X,Y,extend_odd,extend_constant);

    smooth<double> sm(deg);



    {
        ios::ocstream fp("smooth.dat");
        for(size_t i=1;i<=X.size();++i)
        {
            double d = 0;
            const double s = sm(i,data,dx,dx,&d);
            fp("%g %g %g\n", X[i], s, d);
        }
    }


}
Y_UTEST_DONE()

