#include "y/utest/driver.hpp"

Y_UTEST_INIT(32)
{
    Y_UTEST(types);
    Y_UTEST(area);
    Y_UTEST(bitmap);
    Y_UTEST(pixmap);
    Y_UTEST(parallel);
}
Y_UTEST_EXEC()

