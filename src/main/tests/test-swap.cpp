#include "y/type/bswap.hpp"
#include "y/utest/run.hpp"
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

#define __SWAP(Q) do {\
std::cerr << "__bswap<" #Q ">" << std::endl;\
init(); core::__bswap<Q>(arr,brr);\
for(int i=0;i<Q;++i) {\
std::cerr << "\t@" << i << ":"; Y_CHECK(arr[i]==1&&brr[i]==0);\
}\
} while(false)

#define _SWAP(Q) do {\
std::cerr << "_bswap<" #Q ">" << std::endl;\
init(); core::_bswap<Q>(arr,brr);\
for(int i=0;i<Q;++i) {\
std::cerr << "\t@" << i << ":"; Y_CHECK(arr[i]==1&&brr[i]==0);\
}\
} while(false)

    static void do_test()
    {
        __SWAP(0);
        __SWAP(1);
        __SWAP(2);
        __SWAP(3);
        __SWAP(4);
        __SWAP(5);
        __SWAP(6);
        __SWAP(7);


        _SWAP(0); _SWAP(1); _SWAP(2); _SWAP(3); _SWAP(4); _SWAP(5); _SWAP(6);  _SWAP(7); _SWAP(8); _SWAP(9);
        _SWAP(10); _SWAP(11); _SWAP(12); _SWAP(13); _SWAP(14); _SWAP(15); _SWAP(16);  _SWAP(17); _SWAP(18); _SWAP(19);
        _SWAP(20); _SWAP(21); _SWAP(22); _SWAP(23); _SWAP(24); _SWAP(25); _SWAP(26);  _SWAP(27); _SWAP(28); _SWAP(29);
        _SWAP(30); _SWAP(31); _SWAP(32);
    }
}

Y_UTEST(swap)
{
    do_test();
}
Y_UTEST_DONE()

