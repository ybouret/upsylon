
#include "y/mkl/signal/extend.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/types.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

using namespace upsylon;
using namespace mkl;

Y_UTEST(extend)
{
    vector<double> X;
    vector<double> Y;

    X << 0;
    Y << cos(X.back())-0.1;

    X << mkl::numeric<double>::two_pi;
    Y << cos(X.back())-0.1;

    for(size_t i=1+alea.leq(20);i>0;--i)
    {
        X << mkl::numeric<double>::two_pi * alea.to<double>();
        Y << cos(X.back())-0.1;
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
        const unit_t  ilo = -2*unit_t(X.size());
        const unit_t  ihi = -ilo;
        ios::ocstream fp("xtend.dat");
        extend<double>  xconstant(X,Y,extend_constant, extend_constant);
        extend<double>  xeven(    X,Y,extend_even,     extend_even);
        extend<double>  xodd(     X,Y,extend_odd,      extend_odd);
        extend<double>  xcyclic(  X,Y,extend_cyclic,   extend_cyclic);

        {
            ios::ocstream fp("xconst.dat");
            for(unit_t i=ilo;i<=ihi;++i)
            {
                fp("%g %g\n",  xconstant.getX(i),  xconstant.getY(i) );
            }
        }

        {
            ios::ocstream fp("xeven.dat");
            for(unit_t i=ilo;i<=ihi;++i)
            {
                fp("%g %g\n",  xeven.getX(i),  xeven.getY(i) );
            }
        }

        {
            ios::ocstream fp("xodd.dat");
            for(unit_t i=ilo;i<=ihi;++i)
            {
                fp("%g %g\n",  xodd.getX(i),  xodd.getY(i) );
            }
        }


        {
            ios::ocstream fp("xcyclic.dat");
            for(unit_t i=ilo;i<=ihi;++i)
            {
                fp("%g %g\n",  xcyclic.getX(i),  xcyclic.getY(i) );
            }
        }

        
    }



    
}
Y_UTEST_DONE()

