#include "y/type/ints-chkbnd.hpp"
#include "y/os/endian.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

template <typename T>
static inline void check_positive(const char *id)
{
    std::cerr << id << std::endl;
    for(size_t i=0;i<8;++i)
    {
        const T x = alea.full<T>();
        const T y = limit_of<T>::positive(x);
        std::cerr << "pos(" << x << ")=" << y << std::endl;
    }
    std::cerr << std::endl;
}

#define CHECK_POS(TYPE) check_positive<TYPE>( #TYPE )

#define SHOW(OUTPUT,INPUT,VALUE) do { const OUTPUT res = check_bound<OUTPUT,INPUT>(VALUE);\
std::cerr << "check_bound<" << #OUTPUT << "," << #INPUT << ">(" << VALUE << ")=" << res << std::endl; } while(false)

template <typename OUTPUT,typename UNSIGNED_INPUT>
void check_full()
{
    uint64_t j = 0;
    for(UNSIGNED_INPUT i=0; j<limit_of<UNSIGNED_INPUT>::maximum; ++j, ++i)
    {
        core::chkbnd<OUTPUT,UNSIGNED_INPUT>(i);
    }
}


Y_UTEST(chkbnd)
{
    check_full<uint8_t,uint8_t>();
    check_full<uint16_t,uint8_t>();
    check_full<uint32_t,uint8_t>();
    check_full<uint64_t,uint8_t>();

    check_full<int8_t,uint8_t>();
    check_full<int16_t,uint8_t>();
    check_full<int32_t,uint8_t>();
    check_full<int64_t,uint8_t>();


    //CHECK_POS(uint8_t);
    CHECK_POS(uint16_t);
    CHECK_POS(uint32_t);
    CHECK_POS(uint64_t);

    //CHECK_POS(int8_t);
    CHECK_POS(int16_t);
    CHECK_POS(int32_t);
    CHECK_POS(int64_t);

    CHECK_POS(unsigned short);
    CHECK_POS(short);

    CHECK_POS(unsigned int);
    CHECK_POS(int);

    CHECK_POS(unsigned long);
    CHECK_POS(long);

    SHOW(short,int,10000);
    SHOW(short,int,-10000);
    SHOW(short,int,100000);
    SHOW(unsigned short,int,100000);
    SHOW(short,unsigned short,38000);

    const uint32_t X = 0x12345678;
    union
    {
        uint32_t dw;
        uint8_t  b[4];
    } item = { swap_be(X) };
    std::cerr << std::hex << "b=0x" << uint32_t(item.b[0]) << uint32_t(item.b[1]) << int32_t(item.b[2]) << uint32_t(item.b[3]) << std::endl;




}
Y_UTEST_DONE()

