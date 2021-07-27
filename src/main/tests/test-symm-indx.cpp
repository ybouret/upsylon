#include "y/yap.hpp"
#include "y/counting/symm-indx.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/code/human-readable.hpp"
#include "y/string/convert.hpp"
#include "y/type/rtti.hpp"
#include <cstdio>
using namespace upsylon;

namespace  {


    template <typename T>
    static inline void doCheck()
    {
        std::cerr << rtti::name_of<T>() << " : [";
        size_t nmax = 256;
        if( typeid(T) == typeid(apn) )
        {
            nmax = 32;
        }
        for(size_t n=1;n<=nmax;++n)
        {
            (std::cerr << ".").flush();
            const T kmax = ( n*(n+1) )/2;
            for(T k=1;k<=kmax;++k)
            {
                T i,j;
                symm_indx::get_v1(i,j,k);
                Y_ASSERT(1<=i); Y_ASSERT(i<=n);
                Y_ASSERT(1<=j); Y_ASSERT(j<=i);
                Y_ASSERT(i*(i-1)/2+j==k);

                T i2,j2;
                symm_indx::get_v2(i2,j2,k);
                Y_ASSERT(i2==i);
                Y_ASSERT(j2==j);

                T im,jm;
                symm_indx::getC_v1(im,jm,k-1);
                Y_ASSERT(i-1==im);
                Y_ASSERT(j-1==jm);

#if 0
                symm_indx::getC_v2(im,jm,k-1);
                Y_ASSERT(i-1==im);
                Y_ASSERT(j-1==jm);
#endif


            }
        }
        std::cerr << "]" << std::endl;
    }

    template <typename T>
    static inline void doCheckTab()
    {
        std::cerr << rtti::name_of<T>() << " : [";
        for(T k=0;k<sizeof(symm_indx::pair);++k)
        {
            if(0==( (k+1) % 1024 ) ) (std::cerr << '.').flush();

            {
                T i1,j1;
                symm_indx::getC_v1(i1,j1,k);

                T i2,j2;
                symm_indx::getC_v2(i2,j2,k);

                Y_ASSERT(i1==i2);
                Y_ASSERT(j1==j2);
            }

            {
                T i1,j1;
                symm_indx::get_v1(i1,j1,k+1);

                T i3,j3;
                symm_indx::get_v3(i3,j3,k+1);

                Y_ASSERT(i1==i3);
                Y_ASSERT(j1==j3);
            }


        }
        std::cerr << "]" << std::endl;
    }


}

#include "y/ios/ocstream.hpp"

Y_UTEST(symm_indx)
{

    std::cerr << "sizeof(symm_indx::pair)=" << sizeof(symm_indx::pair) << std::endl;
    if(false)
    {
        const unsigned kmax = 32768;
        unsigned       imax,jmax;
        symm_indx::get_v1(imax,jmax,kmax);
        std::cerr << "imax=" << imax << ", jmax=" << jmax << std::endl;

        ios::ocstream fp("symm-indx.hxx");
        fp("const uint8_t symm_indx::pair[%u][2]={\n",kmax);
        for(unsigned km=0;km<kmax;++km)
        {
            const unsigned k = km+1;
            unsigned       i,j;
            symm_indx::get_v1(i,j,k);
            Y_ASSERT(1<=i); Y_ASSERT(i<=256);
            Y_ASSERT(1<=j); Y_ASSERT(j<=256);
            fp(" {%3u,%3u}",i-1,j-1);
            if(k<kmax) fp << ',';
            if(0==(k%16)) fp << '\n';
        }
        fp("};\n");

    }

    std::cerr << "-- Checking Full Match" << std::endl;
    doCheck<uint32_t>();
    doCheck<uint64_t>();
    doCheck<apn>();

    std::cerr << "-- Checking C Match with Table" << std::endl;
    doCheckTab<uint32_t>();
    doCheckTab<uint64_t>();





}
Y_UTEST_DONE()

Y_UTEST(symm_perf)
{
    {
        size_t kmax     = 65536;
        double duration = 1;

        ios::ocstream::overwrite("symm-perf.dat");
        volatile size_t I=0,J=0;
        for(size_t k=1;k<=kmax;k *= 2)
        {
            std::cerr << "@" << k << std::endl;

            double speed1 = 0;
            Y_TIMINGS(speed1,duration,symm_indx::get_v1<volatile size_t>(I,J,k));
            std::cerr << "\tspeed1: " << human_readable(speed1) << std::endl;

            double speed2 = 0;
            Y_TIMINGS(speed2,duration,symm_indx::get_v2<volatile size_t>(I,J,k));
            std::cerr << "\tspeed2: " << human_readable(speed2) << std::endl;


            double speed3 = 0;
            Y_TIMINGS(speed3,duration,symm_indx::get_v3<volatile size_t>(I,J,k));
            std::cerr << "\tspeed3: " << human_readable(speed3) << std::endl;

            ios::ocstream::echo("symm-perf.dat","%g %g %g %g\n", log2( double(k) ), log10(speed1), log10(speed2), log10(speed3) );
        }



    }
}
Y_UTEST_DONE()

