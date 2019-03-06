#include "y/utest/driver.hpp"

Y_UTEST_INIT(8)
{
    Y_UTEST(rules);
    Y_UTEST(grammar);
    Y_UTEST(parser);
    Y_UTEST(ee);
    Y_UTEST(json);
}
Y_UTEST_EXEC()

