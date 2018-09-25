#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(species);
    Y_UTEST(lib);
    Y_UTEST(eq);
    Y_UTEST(cs);
    Y_UTEST(balance);
    Y_UTEST(normalize);
    Y_UTEST(boot);
    Y_UTEST(bootAB);
    Y_UTEST(lua);
    
}
Y_UTEST_EXEC()

