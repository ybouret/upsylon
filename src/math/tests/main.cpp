#include "y/utest/driver.hpp"

Y_UTEST_INIT(64)
{
    Y_UTEST(types);
    Y_UTEST(tao);
    Y_UTEST(tao2);
    Y_UTEST(LU);
    Y_UTEST(data_set);
}
Y_UTEST_EXEC()

