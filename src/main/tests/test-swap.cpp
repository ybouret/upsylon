#include "y/type/bswap.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include <cstring>
#include <cstdio>

using namespace upsylon;
namespace
{
    static const size_t N=128;
    static uint8_t arr[N] = {};
    static uint8_t brr[N] = {};

    static void init() throw()
    {
        memset(arr,0,N);
        for(size_t i=0;i<N;++i)
            brr[i] = uint8_t(i+1);
    }



#define _SWAP(Q) do {\
std::cerr << "bswap<" #Q ">" << std::endl;\
init(); core::bswap<Q>(arr,brr);\
for(int i=0;i<Q;++i) {\
Y_ASSERT(arr[i]==uint8_t(i+1)&&brr[i]==0);\
}\
double speed=0;\
Y_TIMINGS(speed,D,core::bswap<Q>(arr,brr));\
speed /= (1e6);\
std::cerr << "speed" #Q "=" << speed << std::endl;\
if(fp) fprintf(fp,#Q " %.15g\n",speed);\
} while(false)

    static void do_test(const double D)
    {

        FILE *fp = fopen("bswap.dat","wb");

        _SWAP(0);  _SWAP(1);  _SWAP(2);  _SWAP(3);  _SWAP(4);  _SWAP(5);  _SWAP(6);   _SWAP(7);  _SWAP(8);  _SWAP(9);
        _SWAP(10); _SWAP(11); _SWAP(12); _SWAP(13); _SWAP(14); _SWAP(15); _SWAP(16);  _SWAP(17); _SWAP(18); _SWAP(19);
        _SWAP(20); _SWAP(21); _SWAP(22); _SWAP(23); _SWAP(24); _SWAP(25); _SWAP(26);  _SWAP(27); _SWAP(28); _SWAP(29);
        _SWAP(30); _SWAP(31); _SWAP(32); _SWAP(33); _SWAP(34); _SWAP(35); _SWAP(36);  _SWAP(37); _SWAP(38); _SWAP(39);
        _SWAP(40); _SWAP(41); _SWAP(42); _SWAP(43); _SWAP(44); _SWAP(45); _SWAP(46);  _SWAP(47); _SWAP(48); _SWAP(49);
        _SWAP(50); _SWAP(51); _SWAP(52); _SWAP(53); _SWAP(54); _SWAP(55); _SWAP(56);  _SWAP(57); _SWAP(58); _SWAP(59);
        _SWAP(60); _SWAP(61); _SWAP(62); _SWAP(63); _SWAP(64);
        fclose(fp);

    }
}

Y_UTEST(swap)
{
    double D = 0.01;
    if(argc>1) D=atof(argv[1]);
    do_test(D);
}
Y_UTEST_DONE()

