#include "y/type/ints-chkbnd.hpp"
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

Y_UTEST(chkbnd)
{
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
    SHOW(short,unsigned short,38000);
    

}
Y_UTEST_DONE()

