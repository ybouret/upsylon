#include "y/memory/arena.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(arena)
{
    memory::arena A(1,0);

    A.acquire();
}
Y_UTEST_DONE()

