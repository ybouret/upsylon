#include "y/mpl/natural.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/triplet.hpp"

#include <cmath>

using namespace upsylon;

typedef  math::triplet<double> Triplet;

namespace
{
    static const Triplet coeff = {log(2.0),2.5 - 3 * log(2.0), 1.5 - 2 * log(2.0) };
    static       Triplet COEFF = {1,1,1};


    static const double  Xmax  = (12.0*log(2.0)-8.0)/(9.0-12.0*log(2.0));

    static void computeCOEFF(const unsigned p)
    {
        const double fac = ipower(2.0,p);
        COEFF.a = ceil(  fac * coeff.a );
        COEFF.b = floor( fac * coeff.b );
        COEFF.c = ceil(  fac * coeff.c );
    }

    static double computeDELTA( const unsigned p )
    {
        computeCOEFF(p);
        return floor( (ipower(2.0,p)  + COEFF.c - (COEFF.a+COEFF.b)) + 0.5 );
    }


    struct iLog
    {
        unsigned i;
        double operator()(double x) const
        {
            const double x_i = ipower(2.0,i);
            const double dx  = (x-x_i)/x_i;
            return i * coeff.a + dx - coeff.b * (dx*dx) + coeff.c * (dx*dx*dx);
        }

    };

    static double Lambda( double x, unsigned i, unsigned p )
    {
        computeCOEFF(p);
        const double x_i = ipower(2.0,i);
        const double dx  = x-x_i;
        const double num = i*COEFF.a*ipower(2.0,3*i) + dx * ( ipower(2.0,2*i+p) + dx * ( dx*COEFF.c - COEFF.b * ipower(2.0,i) ) );

        return num / ipower(2.0,3*i+p);
    }


    template <typename T>
    static inline void check_round()
    {
        const unsigned nb = sizeof(T)*8;
        for(unsigned ibit=0;ibit<nb;++ibit)
        {
            const T round_one  = 1;
            const T round_word = round_one << ibit;
            const T round_mask = round_word - round_one;
            const T round_not2 = ~round_mask;

            std::cerr << std::dec << "ibit=" << ibit;
            std::cerr << std::hex << "\tword=" << round_word << ", mask=" << round_mask << ", not2=" <<  round_not2;

            std::cerr << std::endl;
        }
        std::cerr << std::dec;
    }

}

Y_UTEST(ilog)
{

    check_round<uint16_t>();


    std::cerr << "coeff=" << coeff << std::endl;
    {
        ios::ocstream fp("delta.dat");
        for(unsigned p=0; p <= 32; ++p )
        {
            const double d = computeDELTA(p);
            if(d<=0)
            {
                std::cerr << "COEFF" << p << "=" << COEFF << std::endl;
                fp("%u %g\n",p,d);
            }
        }
    }

    iLog L;
    {
        ios::ocstream mx("deltamax.dat");
        ios::ocstream fp("logapprox.dat");
        unsigned &i = L.i;
        for( i=0; i <= 16; ++i )
        {
            const unsigned xlo = (1<<i);
            const unsigned xup = xlo<<1;
            for(unsigned x=xlo;x<=xup;++x)
            {
                const double Lx = L(x);
                fp("%u %g %g\n", x, Lx, Lx-log(double(x)));
            }
            const double xmax = Xmax * xlo + xlo;
            const double Lmax = L(xmax);
            const double dmax = L(xmax) - log(xmax);
            mx("%g %g %g\n", xmax, Lmax, dmax);
        }
    }

    static const unsigned p_val[] = { 3,5,11,13,14,16,17 };
    static const unsigned p_num = sizeof(p_val)/sizeof(p_val[0]);
    for(unsigned ip=0; ip < p_num; ++ip )
    {
        const unsigned p = p_val[ip];
        const double   d = computeDELTA(p);
        if(d>0) throw exception("unexpected bad delta...");
        const string  fn = vformat("ilog%u.dat",p);
        ios::ocstream fp(fn);
        unsigned &i = L.i;
        for( i=0;i<=10;++i)
        {
            const unsigned xlo = (1<<i);
            const unsigned xup = xlo<<1;
            for(unsigned x=xlo;x<=xup;++x)
            {
                //const double Lx  = L(x);
                const double Lam = Lambda(x,i,p);
                fp("%g %g %g\n", log(double(x)), Lam, Lam-log(double(x)));
            }
        }
    }


}
Y_UTEST_DONE()

