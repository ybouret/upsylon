#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(platform)
{
    std::cerr << "sizeof(exception)=" << sizeof(upsylon::exception) << std::endl;
    Y_CHECK(sizeof(int8_t)==1);
    Y_CHECK(sizeof(int16_t)==2);
    Y_CHECK(sizeof(int32_t)==4);
    Y_CHECK(sizeof(int64_t)==8);
    Y_CHECK(sizeof(uint8_t)==1);
    Y_CHECK(sizeof(uint16_t)==2);
    Y_CHECK(sizeof(uint32_t)==4);
    Y_CHECK(sizeof(uint64_t)==8);
}
Y_UTEST_DONE()
