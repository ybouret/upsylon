
#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(init);
    Y_UTEST(io);
    Y_UTEST(star);
    Y_UTEST(ring);
    Y_UTEST(PI);
}
Y_UTEST_EXEC()
