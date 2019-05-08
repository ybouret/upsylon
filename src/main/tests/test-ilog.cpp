#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"
#include "y/os/progress.hpp"

#include <cmath>

using namespace upsylon;


namespace
{

    double S_(unsigned k, const double y )
    {
        const   unsigned tkp1 = k+k+1;
        return  2.0/tkp1 * ipower((y-1)/(y+1),tkp1);
    }

    double L_(unsigned k, const double y)
    {
        double ans = 0;
        while(k-- > 0 )
        {
            ans += S_(k,y);
        }
        return ans;
    }


}

Y_UTEST(ilog)
{
    progress       bar;
    const unsigned nmax=10000;

    ios::ocstream::overwrite("ilog.dat");
    bar.start();
    double t_old = bar.query();
    for(unsigned y=1;y<=nmax;++y)
    {
        const double yd = y;
        const double ly = log(yd);

        unsigned k   = 0;
        double   sum = S_(0,yd);
        while( ly-sum > 1.0 )
        {
            sum += S_(++k,yd);
        }
        const double lsa = L_(k,y);
        ios::ocstream::echo("ilog.dat","%u %g %g %u %g\n",y,ly,ly-sum,k,lsa);

        const double t_now = bar.query();
        if(t_now-t_old>=0.2)
        {
            bar.update(y,nmax);
            (bar.display(std::cerr) << "\r").flush();
            t_old=t_now;
        }
    }
    bar.update(1);
    std::cerr << std::endl;





}
Y_UTEST_DONE()

