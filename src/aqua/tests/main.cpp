
#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(species);
    Y_UTEST(library);
    Y_UTEST(eqs);
    Y_UTEST(engine);
    Y_UTEST(boot);
    Y_UTEST(citric);
    Y_UTEST(lua);
}
Y_UTEST_EXEC()

