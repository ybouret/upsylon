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
        std::cerr << "Checking : [";
        for(size_t n=1;n<=200;++n)
        {
            (std::cerr << ".").flush();
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

                size_t i3,j3;
                symm_indx::get_v3(i3,j3,k);
                Y_ASSERT(i3==i);
                Y_ASSERT(j3==j);


                size_t im,jm;
                symm_indx::getC(im,jm,k-1);
                Y_ASSERT(i-1==im);
                Y_ASSERT(j-1==jm);
            }
        }
        std::cerr << "]" << std::endl;
    }


}

#include "y/ios/ocstream.hpp"

Y_UTEST(symm_indx)
{
    doCheck();
    

    double       D    = 1.0;
    size_t       KMAX = 10000;
    if(argc>1)
    {
        KMAX = string_convert::to<size_t>(argv[1],"KMAX");
    }
    if(argc>2)
    {
        D = string_convert::to<double>(argv[2],"D");
    }
    volatile     size_t I=0,J=0;

    ios::ocstream::overwrite("symm_indx.dat");
    for(size_t kmax=2;kmax<=KMAX;kmax*=2)
    {
        std::cerr << "kmax=" << kmax << std::endl;
        double speed1 = 0;
#define CODE1 for(size_t k=1;k<=kmax;++k) symm_indx::get_v1<volatile size_t>(  I,  J,k)
        Y_TIMINGS(speed1,D,CODE1);
        std::cerr << "speed1=" << human_readable(speed1) << std::endl;

        double speed2 = 0;
#define CODE2 for(size_t k=1;k<=kmax;++k) symm_indx::get_v2<volatile size_t>(  I,  J,k)
        Y_TIMINGS(speed2,D,CODE2);
        std::cerr << "speed2=" << human_readable(speed2)  << std::endl;

        double speed3 = 0;
#define CODE3 for(size_t k=1;k<=kmax;++k) symm_indx::get_v3<volatile size_t>(  I,  J,k)
        Y_TIMINGS(speed3,D,CODE3);
        std::cerr << "speed3=" << human_readable(speed3)  << std::endl;

        ios::ocstream::echo("symm_indx.dat","%g %g %g %g\n", log10( double(kmax) ), log10(speed1), log10(speed2), log10(speed3) );
    }
    std::cerr << "plot 'symm_indx.dat' u 1:2 w lp title \"isqrt\", '' u 1:3 w lp title \"linear\", '' u 1:4 w lp title \"bis\"" << std::endl;

}
Y_UTEST_DONE()

