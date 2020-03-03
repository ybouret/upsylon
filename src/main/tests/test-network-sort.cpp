#include "y/sort/network/all.hpp"
#include "y/comparison.hpp"
#include "y/utest/run.hpp"

#include <cstdlib>
#include <iomanip>
#include "y/os/wtime.hpp"
#include <cstring>
#include "y/code/hr-ints.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace {
    
    template <size_t N,typename T>
    void doSort()
    {
        wtime chrono;
        std::cerr << "  N=" << std::setw(2) << N << " [";
        T a[N];
        T b[N];
        
        uint64_t qs = 0;
        uint64_t nw = 0;
        
        const size_t iterMax = 64;
        for(size_t iter=iterMax;iter>0;--iter)
        {
            if(0==(iter&3))
            {
                std::cerr << "."; std::cerr.flush();
            }
            
            alea.fill(a,sizeof(a));
            memcpy(b,a,sizeof(a));
            {
                const uint64_t mark = wtime::ticks();
                qsort(a, N, sizeof(T), comparison::__increasing<T> );
                qs += wtime::ticks() - mark;
            }
            {
                const uint64_t mark = wtime::ticks();
                network_sort<N>::on(b, comparison::increasing<T>   );
                nw += wtime::ticks() - mark;
            }
            for(size_t i=0;i<N;++i)
            {
                //std::cerr << a[i] << '/' << b[i] << std::endl;
                Y_ASSERT(a[i]==b[i]);
            }
        }
        const double ops      = N * iterMax;
        const double tqs      = chrono(qs);
        const double tnw      = chrono(nw);
        const int64_t qs_speed = int64_t(floor(ops/tqs+0.5));
        const int64_t nw_speed = int64_t(floor(ops/tnw+0.5));
        const human_readable hrqs = qs_speed; const string sqs = vformat("%8.2lf%c/s",hrqs.value,hrqs.radix);
        const human_readable hrnw = nw_speed; const string snw = vformat("%8.2lf%c/s",hrnw.value,hrnw.radix);
        std::cerr << "] : qsort=" << sqs << " | nwsrt=" << snw << std::endl;


    }
    
    template<typename T>
    void doSortAll()
    {
        std::cerr << "blockSize=" << sizeof(T) << std::endl;
        doSort<2,T>();
        doSort<3,T>();
        doSort<4,T>();
        doSort<5,T>();
        doSort<6,T>();
        doSort<7,T>();
        doSort<8,T>();
        doSort<9,T>();
        doSort<10,T>();
        doSort<11,T>();
        doSort<12,T>();
        doSort<13,T>();
        doSort<14,T>();
        doSort<15,T>();
        doSort<16,T>();
        doSort<17,T>();
        doSort<18,T>();
        doSort<19,T>();
        doSort<20,T>();
        doSort<21,T>();
        doSort<22,T>();
        doSort<23,T>();
        doSort<24,T>();
        doSort<25,T>();
        doSort<26,T>();
        doSort<27,T>();
        doSort<28,T>();
        doSort<29,T>();
        doSort<30,T>();
        doSort<31,T>();
        doSort<32,T>();
        
    }
    
}

Y_UTEST(network_sort)
{
    doSortAll<int8_t>();
    doSortAll<int16_t>();
    doSortAll<int32_t>();
    doSortAll<int64_t>();

}
Y_UTEST_DONE()



