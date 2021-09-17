
#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(types);
    Y_UTEST(lib);
    Y_UTEST(actors);
    Y_UTEST(eqs);
    Y_UTEST(sys);
    Y_UTEST(lua);
    Y_UTEST(torture);
}
Y_UTEST_EXEC()
