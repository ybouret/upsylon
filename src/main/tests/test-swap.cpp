#include "y/type/bswap.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include <cstring>

using namespace upsylon;
namespace
{
    static const size_t N=128;
    static uint8_t arr[N] = {};
    static uint8_t brr[N] = {};

    static void init() throw()
    {
        memset(arr,0,N);
        memset(brr,1,N);
    }

#if 0
#define __SWAP(Q) do {\
std::cerr << "__bswap<" #Q ">" << std::endl;\
init(); core::bswap<Q>(arr,brr);\
for(int i=0;i<Q;++i) {\
std::cerr << "\t@" << i << ":"; Y_CHECK(arr[i]==1&&brr[i]==0);\
}\
} while(false)
#endif

#define _SWAP(Q) do {\
std::cerr << "bswap<" #Q ">" << std::endl;\
init(); core::bswap<Q>(arr,brr);\
for(int i=0;i<Q;++i) {\
std::cerr << "\t@" << i << ":"; Y_CHECK(arr[i]==1&&brr[i]==0);\
}\
double speed=0;\
Y_TIMINGS(speed,0.1,core::bswap<Q>(arr,brr));\
speed /= 1e6;\
std::cerr << "speed" #Q "=" << speed << std::endl;\
if(fp) fprintf(fp,#Q " %.15g\n",speed);\
} while(false)

    static void do_test()
    {
#if 0
        __SWAP(0);
        __SWAP(1);
        __SWAP(2);
        __SWAP(3);
        __SWAP(4);
        __SWAP(5);
        __SWAP(6);
        __SWAP(7);
#endif
        FILE *fp = fopen("bswap.dat","wb");

        _SWAP(0);  _SWAP(1);  _SWAP(2);  _SWAP(3); _SWAP(4); _SWAP(5); _SWAP(6);  _SWAP(7); _SWAP(8); _SWAP(9);
        _SWAP(10); _SWAP(11); _SWAP(12); _SWAP(13); _SWAP(14); _SWAP(15); _SWAP(16);  _SWAP(17); _SWAP(18); _SWAP(19);
        _SWAP(20); _SWAP(21); _SWAP(22); _SWAP(23); _SWAP(24); _SWAP(25); _SWAP(26);  _SWAP(27); _SWAP(28); _SWAP(29);
        _SWAP(30); _SWAP(31); _SWAP(32); _SWAP(33); _SWAP(34);

        fclose(fp);

    }
}

Y_UTEST(swap)
{
    do_test();
}
Y_UTEST_DONE()

