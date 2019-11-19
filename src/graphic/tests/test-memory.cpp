
#include "y/graphic/memory.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(memory)
{
    char       data1[] = "Hello, World!";
    const char data2[] = "Hello, World!";

    Memory m1(data1,sizeof(data1)); m1.displayMemoryInfo();
    Memory m2(data2,sizeof(data2)); m2.displayMemoryInfo();
    Memory m3(100, Memory::RO);     m3.displayMemoryInfo();
    Memory m4(200, Memory::RW);     m4.displayMemoryInfo();

    Y_CHECK( Memory::RW == m1.mode);
    Y_CHECK( Memory::RO == m2.mode);
    Y_CHECK( Memory::RO == m3.mode);
    Y_CHECK( Memory::RW == m4.mode);

    {
        Memory mm1(m1); mm1.displayMemoryInfo();
        Memory mm2(m2); mm2.displayMemoryInfo();
        Memory mm3(m3); mm3.displayMemoryInfo();
        Memory mm4(m4); mm4.displayMemoryInfo();
    }

}
Y_UTEST_DONE()

