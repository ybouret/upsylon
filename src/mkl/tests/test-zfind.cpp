#include "y/mkl/fcn/zfind.hpp"
#include "y/mkl/fcn/functions.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{
    template <typename T>
    struct Call
    {
        unsigned calls;
        inline T operator()(const T x)
        {
            ++calls;
            return T(0.1)+cos_of(x*x);
        }
    };

    template <typename T>
    struct iQerf
    {
        unsigned calls;
        inline T operator()(const T x)
        {
            ++calls;
            return qerf(x);
        }
    };

}

Y_UTEST(zfind)
{
    Call<double> F;


    std::cerr << "bisection: " << std::endl;
    {
        triplet<double> x = {0, -1, 2};
        triplet<double> f = {F(x.a),-1,F(x.c)};
        std::cerr << "x=" << x << std::endl;
        std::cerr << "f=" << f << std::endl;
        F.calls = 0;
        if(zfind::_bisection(F,x,f))
        {
            std::cerr << "F(" << x.b << ")=" << f.b << std::endl;
            std::cerr << "|_#calls=" << F.calls << std::endl;
        }
        else
        {
            std::cerr << "couldn't find zero" << std::endl;
        }
        const double x2 = zfind::get(0.12,F,0.0,2.0,zfind::ridder);
        std::cerr << "x2=" << x2 << std::endl;
        std::cerr << "F2=" << F(x2) << std::endl;
        
        std::cerr << "Testing..." << std::endl;
        size_t bis_calls = 0;
        size_t rid_calls = 0;
        for(size_t iter=0;iter<20;++iter)
        {
            x.a = 5*alea.symm<double>();
            f.a = F(x.a);
            x.c = x.a;
            while( (f.c=F( x.c += 0.01* alea.to<double>() )) * f.a > 0 )
                ;
            if( alea.choice() )
            {
                cswap(x.a,x.c);
                cswap(f.a,f.c);
            }
            
            triplet<double> xx = { x.a, 0, x.c };
            triplet<double> ff = { f.a, 0, f.c };
            //std::cerr << "@" << xx << " : " << ff << std::endl;
            F.calls = 0; Y_ASSERT( zfind::_bisection(F,x,f) ); bis_calls += F.calls;
            F.calls = 0; Y_ASSERT( zfind::_ridder(F,xx,ff) );  rid_calls += F.calls;
            std::cerr << x.b << " " << xx.b << " -> " << fabs_of(x.b-xx.b) << std::endl;
        }
        std::cerr << "\t " << bis_calls << " " << rid_calls << std::endl;
    }


    std::cerr << "ridder:" << std::endl;
    {
        triplet<double> x = {0, -1, 2};
        triplet<double> f = {F(x.a),-1,F(x.c)};
        std::cerr << "x=" << x << std::endl;
        std::cerr << "f=" << f << std::endl;
        F.calls = 0;
        if( zfind::_ridder(F, x, f) )
        {
            std::cerr << "F(" << x.b << ")=" << f.b << std::endl;
            std::cerr << "|_#calls=" << F.calls << std::endl;
        }
        else
        {
            std::cerr << "couldn't find zero" << std::endl;

        }
    }

#if 1
    if(true)
    {
        iQerf<float> Z;
        Z.calls = 0;
        for(float y=-0.9;y<=0.9;y+=0.2)
        {
            const float x = zfind::get(y, Z, -10.0f, 10.0f, zfind::bisection);
            std::cerr << "iqerf(" << y << ")="  << x << std::endl;
        }
        std::cerr << "#calls=" << Z.calls << std::endl;

        Z.calls = 0;
        for(float y=-0.9;y<=0.9;y+=0.2)
        {
            const float x = zfind::get(y, Z, -10.0f, 10.0f, zfind::ridder);
            std::cerr << "iqerf(" << y << ")="  << x << std::endl;
        }
        std::cerr << "#calls=" << Z.calls << std::endl;
    }
#endif

}
Y_UTEST_DONE()

#include <iomanip>

namespace {

    template <typename T>
    void test_lfind()
    {

        static const T      ff[] = { T(-0.1), T(0.0), T(0.1) };
        static const size_t nn = sizeof(ff)/sizeof(ff[0]);
        triplet<T> x;
        triplet<T> f;

        std::cerr << std::setprecision(3);

        for(size_t i=0;i<nn;++i)
        {
            f.a = ff[i];
            for(size_t j=0;j<nn;++j)
            {
                f.c = ff[j];
                x.a = alea.symm<T>();
                x.c = alea.symm<T>();
                f.b = 0;
                std::cerr << "f:" << f << "\t";
                if(f.a*f.c<=0)
                {
                    std::cerr << "+";
                    Y_ASSERT(zfind::linear(x,f));
                }
                else
                {
                    std::cerr << "-";
                }
                std::cerr << std::endl;
            }
        }

        for(size_t iter=1024*16;iter>0;--iter)
        {
            x.a = alea.symm<T>();
            x.c = alea.symm<T>();

            f.a = alea.symm<T>();
            f.c = alea.symm<T>();

            if( alea.to<double>() > 0.85 )
            {
                if( alea.choice() )
                {
                    f.a=0;
                }
                else
                {
                    f.b=0;
                }
            }

            if(f.a*f.c<=0)
            {
                Y_ASSERT(zfind::linear(x,f));
            }
        }

    }
}

Y_UTEST(lfind)
{
    test_lfind<float>();
    test_lfind<double>();
}
Y_UTEST_DONE()
