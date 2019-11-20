
#include "y/graphic/memory.hpp"
#include "y/utest/run.hpp"
#include "y/type/bzset.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(memory)
{
    const char data1[] = "Hello, World!";
    char       data2[] = "Run Forrest, run!";

    Metrics mtx1 = { data1, sizeof(data1), Metrics::ReadOnly | Metrics::StaticMemory, sizeof(data1), 1, 1, sizeof(data1) };
    Metrics mtx2 = { data2, sizeof(data1), Metrics::ReadWrite| Metrics::StaticMemory, sizeof(data2), 1, 1, sizeof(data2) };
    Memory  m1(mtx1); m1.displayMemory();
    Memory  m2(mtx2); m2.displayMemory();

    Metrics mtx3; bzset(mtx3);
    mtx3.count = alea.leq(100);
    mtx3.entry = Metrics::Acquire(mtx3.count);
    mtx3.flags = Metrics::ReadWrite | Metrics::GlobalMemory;
    Memory m3( mtx3 ); m3.displayMemory();

    {
        Memory mm1(m1); mm1.displayMemory();
        Memory mm2(m2); mm2.displayMemory();
        Memory mm3(m3); mm3.displayMemory();
    }

    m1.displayMemory();
    m2.displayMemory();
    m3.displayMemory();

}
Y_UTEST_DONE()

