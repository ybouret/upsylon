
#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(init);
    Y_UTEST(oxide);
    Y_UTEST(io);
    Y_UTEST(topology);
}
Y_UTEST_EXEC()
