#include "y/counting/symm-indx.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/code/human-readable.hpp"
#include "y/string/convert.hpp"
#include <cstdio>
using namespace upsylon;

namespace  {


    static inline void doCheck()
    {
        for(size_t n=1;n<=100;++n)
        {
            const size_t kmax = ( n*(n+1) )/2;
            for(size_t k=1;k<=kmax;++k)
            {
                size_t i,j;
                symm_indx::get_v1(i,j,k);
                Y_ASSERT(1<=i); Y_ASSERT(i<=n);
                Y_ASSERT(1<=j); Y_ASSERT(j<=i);
                Y_ASSERT(i*(i-1)/2+j==k);

                size_t i2,j2;
                symm_indx::get_v2(i2,j2,k);
                Y_ASSERT(i2==i);
                Y_ASSERT(j2==j);

                size_t im,jm;
                symm_indx::getC(im,jm,k-1);
                Y_ASSERT(i-1==im);
                Y_ASSERT(j-1==jm);
            }
        }
    }

    static inline size_t f(const size_t x)
    {
        return x*(x-1);
    }

    size_t find(const size_t y)
    {
        size_t lo = 1;
        size_t hi = y;
        while(hi-lo>1)
        {
            const size_t mid = (lo+hi)/2;
            const size_t val = f(mid);
            if(val>y)
            {
                hi = mid;
            }
            else
            {
                if(val<y)
                {
                    lo = mid;
                }
                else
                {
                    return mid;
                }
            }
        }
        return lo;
    }

}

Y_UTEST(symm_indx)
{
    doCheck();


#if 0
    const double D    = 2.0;
    size_t       KMAX = 10000;
    if(argc>1)
    {
        KMAX = string_convert::to<size_t>(argv[1],"KMAX");
    }
    volatile     size_t I=0,J=0;


    double speed1 = 0;
#define CODE1 for(size_t k=1;k<=KMAX;++k) symm_indx::get<volatile size_t>(  I,  J,k)
    Y_TIMINGS(speed1,D,CODE1);
    std::cerr << "speed1=" << human_readable(speed1) << std::endl;

    double speed2 = 0;
#define CODE2 for(size_t k=1;k<=KMAX;++k) symm_indx::get_v2<volatile size_t>(  I,  J,k)
    Y_TIMINGS(speed2,D,CODE2);
    std::cerr << "speed2=" << human_readable(speed2)  << std::endl;
#endif

    for(size_t k=1;k<=20;++k)
    {
        size_t i,j;
        symm_indx::get_v1(i,j,k);

        const size_t ib = find(2*(k-1));
        std::cerr << "k=" << k << "->" << i << " -> " << ib << std::endl;
    }


}
Y_UTEST_DONE()

