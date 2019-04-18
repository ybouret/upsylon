#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

#include <cmath>

using namespace upsylon;

namespace
{

    static const double a = log(2);
    static const double b = 2.5 - 3 * log(2.0);
    static const double c = 1.5 - 2 * log(2.0);

    class iLog
    {
    public:
        iLog() throw() {}
        ~iLog() throw() {}

        double operator()(size_t x, size_t i, size_t p)
        {
            const size_t tpp   = ipower(2,p);
            const size_t tpi   = ipower(2.0,i);
            const size_t A     = size_t(ceil(  a * tpp ));
            const size_t B     = size_t(floor( b * tpp ));
            const size_t C     = size_t(ceil(  c * tpp ));
            const size_t X     = x-tpi;

            size_t den = i * ipower(2,3*i)*A;
            den       += X * ipower(2,2*i+p);
            den       += C * ipower(X,3);
            den       -= B * ipower(2,i) * ipower(X,2);
            return den/ ipower(2.0,3*i+p);
        }

        double delta( const size_t p )
        {
            const double tpp   = ipower(2.0,p);
            const double A     = ceil(  a * tpp );
            const double B     = floor( b * tpp );
            const double C     = ceil(  c * tpp );
            return tpp-A-B+C;
        }

    private:

    };


}

Y_UTEST(ilog)
{

    std::cerr << "a=" << a << ", b=" << b << ", c=" << c << std::endl;

    iLog Lam;
    {
        ios::ocstream fp("delta.dat");
        for(size_t p=0;p<=30;++p)
        {
            const double delta = Lam.delta(p);
            if(delta<=0)
            {
                const double tpp   = ipower(2.0,p);
                const double A     = ceil(  a * tpp );
                const double B     = floor( b * tpp );
                const double C     = ceil(  c * tpp );
                fp("%u %g %g %g\n", unsigned(p),A,B,C );
            }
        }
    }
    for(size_t p=0;p<=17;++p)
    {
        std::cerr << "p=" << p << ", delta=" << Lam.delta(p) << std::endl;
        {
            const double tpp = ipower(2.0,p);
            const double A   = ceil(  a * tpp );
            const double B   = floor( b * tpp );
            const double C   = ceil(  c * tpp );
            std::cerr << "A=" << A << ", B=" << B << ", C=" << C << std::endl;
        }
        const string  fn  = vformat("ilog%u.dat",unsigned(p));
        ios::ocstream fp(fn);
        for(size_t i=0;i<=16;++i)
        {
            for(size_t x=(1<<i);x<=(1<<(i+1));++x)
            {
                fp("%u %g %g\n", unsigned(x), double(Lam(x,i,p))-log(x), double(Lam(x,i,p)));
            }
        }
    }

    //std::cerr << "plot 'ilog.dat' u 1:2:3 w lp lc var" << std::endl;

}
Y_UTEST_DONE()

