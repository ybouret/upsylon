#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(types);
    Y_UTEST(layout);
    Y_UTEST(partition);
    Y_UTEST(field);
    Y_UTEST(topology);
    Y_UTEST(layouts);
}
Y_UTEST_EXEC()
