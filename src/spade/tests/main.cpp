#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(types);
    Y_UTEST(layout);
    Y_UTEST(field);
    Y_UTEST(topology);
    Y_UTEST(fragment);
    Y_UTEST(polint);
    Y_UTEST(ghosts);
    Y_UTEST(tessellation);
    Y_UTEST(transfer);
    Y_UTEST(workspace);
}
Y_UTEST_EXEC()
