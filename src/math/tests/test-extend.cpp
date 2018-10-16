
#include "y/math/signal/extend.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/types.hpp"
#include "y/sort/heap.hpp"

using namespace upsylon;


Y_UTEST(extend)
{
    vector<double> X;
    vector<double> Y;

    X << 0;
    Y << cos(X.back());

    X << math::numeric<double>::two_pi;
    Y << cos(X.back());

    for(size_t i=1+alea.leq(20);i>0;--i)
    {
        X << math::numeric<double>::two_pi * alea.to<double>();
        Y << cos(X.back());
    }

    hsort(X,Y,comparison::increasing<double>);

    {
        ios::ocstream fp("xdata.dat");
        for(size_t i=1;i<=X.size();++i)
        {
            fp("%g %g\n", X[i], Y[i] );
        }
    }

    {
        const unit_t ilo = -2*unit_t(X.size());
        const unit_t ihi = -ilo;
        ios::ocstream fp("xtend.dat");
        math::extend<double>  xconstant(X,Y,math::extend_constant,math::extend_constant);
        math::extend<double>  xcyclic(X,Y,math::extend_cyclic,math::extend_cyclic);

        for(unit_t i=ilo;i<=ihi;++i)
        {
            fp("%g %g",  xconstant.getX(i),  xconstant.getY(i) );
            fp(" %g %g", xcyclic.getX(i),    xcyclic.getY(i)   );
            fp << "\n";
        }
    }



    
}
Y_UTEST_DONE()

