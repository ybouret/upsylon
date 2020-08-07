#include "y/comparison.hpp"
#include "y/utest/run.hpp"

#include <cstdlib>
#include <iomanip>
#include "y/os/wtime.hpp"
#include <cstring>
#include "y/code/hr-ints.hpp"
#include "y/string.hpp"
#include "y/ios/ocstream.hpp"
#include "y/memory/embed.hpp"
#include "y/memory/allocator/global.hpp"

using namespace upsylon;

#if 1
#include "y/sort/network/all.hpp"

namespace {
    
    template <size_t N,typename T>
    void doSort(ios::ostream &fp)
    {
        wtime chrono;
        std::cerr << "  N=" << std::setw(2) << N << " [";


        const size_t bytes = N*sizeof(T);
        T      *a = 0;
        T      *b = 0;
        size_t *idx = 0;
        void   *wksp = 0;
        size_t  wlen = 0;
        {
            memory::embed emb[] =
            {
                memory::embed::as(a,N),
                memory::embed::as(b,N),
                memory::embed::as(idx,N)
            };
            wksp = memory::embed::create(emb, sizeof(emb)/sizeof(emb[0]), memory::global::instance(), wlen);
        }



        uint64_t     qs = 0;
        uint64_t     nw = 0;
        uint64_t     ops = 0;
        const size_t iterMax = 2048;
        for(size_t iter=iterMax;iter>0;--iter)
        {
            ops += N;
            if( 0 == (iter&31) )
            {
                ( std::cerr << '.' ).flush();
            }
            
            alea.fill(a,bytes);
            out_of_reach::move(b,a,bytes);
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
                Y_ASSERT(a[i]==b[i]);
            }
        }
        const double tqs      = chrono(qs);
        const double tnw      = chrono(nw);
        const int64_t qs_speed = int64_t(floor(double(ops)/tqs+0.5));
        const int64_t nw_speed = int64_t(floor(double(ops)/tnw+0.5));
        const human_readable hrqs = qs_speed; const string sqs = vformat("%8.2lf%c/s",hrqs.value,hrqs.radix);
        const human_readable hrnw = nw_speed; const string snw = vformat("%8.2lf%c/s",hrnw.value,hrnw.radix);
        std::cerr << "] : qsort=" << sqs << " | nwsrt=" << snw << std::endl;

        fp("%u %g %g\n", unsigned(N), 1e-6 * qs_speed, 1e-6 * nw_speed );

        std::cerr.flush();


        for(size_t iter=8;iter>0;--iter)
        {
            alea.fill(a,bytes);
            out_of_reach::move(b,a,bytes);
            out_of_reach::fill(idx,0,sizeof(idx));
            out_of_reach::fill_indices(idx,N,0);
            for(size_t i=0;i<N;++i)
            {
                Y_ASSERT(b[i]   == a[i]);
                Y_ASSERT(idx[i] == i   );
                //std::cerr << "a[" << i << "]=" << int64_t(a[i]) << std::endl;
            }
            network_sort<N>::co(a,idx,comparison::increasing<T>);
            
        }

        memory::global::location().release(wksp, wlen);

    }
    
    template<typename T>
    void doSortAll()
    {
        std::cerr << "blockSize=" << sizeof(T) << std::endl;
        const string  fn = vformat("nws%u.dat", unsigned( sizeof(T) ) );
        ios::ocstream fp(fn);

        doSort<2,T>(fp);
        doSort<3,T>(fp);
        doSort<4,T>(fp);
        doSort<5,T>(fp);
        doSort<6,T>(fp);
        doSort<7,T>(fp);
        doSort<8,T>(fp);
        doSort<9,T>(fp);
        doSort<10,T>(fp);
        doSort<11,T>(fp);
        doSort<12,T>(fp);
        doSort<13,T>(fp);
        doSort<14,T>(fp);
        doSort<15,T>(fp);
        doSort<16,T>(fp);
        doSort<17,T>(fp);
        doSort<18,T>(fp);
        doSort<19,T>(fp);
        doSort<20,T>(fp);
        doSort<21,T>(fp);
        doSort<22,T>(fp);
        doSort<23,T>(fp);
        doSort<24,T>(fp);
        doSort<25,T>(fp);
        doSort<26,T>(fp);
        doSort<27,T>(fp);
        doSort<28,T>(fp);
        doSort<29,T>(fp);
        doSort<30,T>(fp);
        doSort<31,T>(fp);
        doSort<32,T>(fp);

        doSort<33,T>(fp);
        doSort<34,T>(fp);
        doSort<35,T>(fp);
        doSort<36,T>(fp);
        doSort<37,T>(fp);
        doSort<38,T>(fp);
        doSort<39,T>(fp);

        doSort<40,T>(fp);
        doSort<41,T>(fp);
        doSort<42,T>(fp);
        doSort<43,T>(fp);
        doSort<44,T>(fp);
        doSort<45,T>(fp);
        doSort<46,T>(fp);
        doSort<47,T>(fp);
        doSort<48,T>(fp);
        doSort<49,T>(fp);

        doSort<50,T>(fp);
        doSort<51,T>(fp);
        doSort<52,T>(fp);
        doSort<53,T>(fp);
        doSort<54,T>(fp);
        doSort<55,T>(fp);
        doSort<56,T>(fp);
        doSort<57,T>(fp);
        doSort<58,T>(fp);
        doSort<59,T>(fp);

        doSort<60,T>(fp);
        doSort<61,T>(fp);
        doSort<62,T>(fp);
        doSort<63,T>(fp);
        doSort<64,T>(fp);

    }
    
}

#endif

Y_UTEST(network_sort)
{
#if 1
    doSortAll<int8_t>();
    doSortAll<int16_t>();
    doSortAll<int32_t>();
    doSortAll<int64_t>();
#endif
    
}
Y_UTEST_DONE()



