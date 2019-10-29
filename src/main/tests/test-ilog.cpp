#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"
#include "y/os/progress.hpp"
#include "y/core/ipower.hpp"
#include "y/type/utils.hpp"

#include <cmath>

using namespace upsylon;


namespace
{

    struct LnApprox
    {
        static inline double S_(unsigned k, const double y )
        {
            const   unsigned tkp1 = k+k+1;
            return  2.0/tkp1 * ipower((y-1)/(y+1),tkp1);
        }

        static inline double L_(unsigned k, const double y)
        {
            double ans = 0;
            while(k-- > 0 )
            {
                ans += S_(k,y);
            }
            return ans;
        }

        static bool Test(const unsigned  nbits,
                         const unsigned  max_power,
                         double         &max_diff,
                         unsigned       &factor)
        {
            const double   ln2 = log(2.0);
            const double   ip2 = ipower(2.0,nbits);
            const unsigned fac = unsigned( ceil( ipower(2.0,nbits) * ln2 ) );
            factor = fac;
            std::cerr << "Testing for " << fac << "/" << ip2 << std::endl;
            max_diff = 0;
            for(unsigned i=0;i<=max_power;++i)
            {
                const double   iln2 = i*ln2;
                const unsigned appx = ((i*fac)>>nbits)+1;
                if( double(appx) < iln2 )
                {
                    std::cerr << "appx=" << appx << " for iln2=" << iln2 << "@" << i << std::endl;
                    return false;
                }
                max_diff = max_of(max_diff,double(appx)-iln2);
            }
            return true;
        }
    };


}

Y_UTEST(ilog)
{

#if 0
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
#endif

    ios::ocstream::overwrite("ilog.dat");


    for(unsigned nbits=0;nbits<=16;++nbits)
    {
        std::cerr << "nbits=" << nbits << std::endl;
        double     dmax = 0;
        unsigned   lfac = 0;
        const bool ans  = LnApprox::Test(nbits,10000,dmax,lfac);
        if( ans )
        {
            std::cerr << "success" << std::endl;
            ios::ocstream::echo("ilog.dat","%u %g %u\n",nbits,dmax,lfac);

        }
        else
        {
            std::cerr << "failure" << std::endl;
        }
    }



}
Y_UTEST_DONE()

