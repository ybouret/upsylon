#include "y/utest/driver.hpp"

Y_UTEST_INIT(4)
{
    Y_UTEST(platform);
    Y_UTEST(types);
    Y_UTEST(lockable);
    
}
Y_UTEST_EXEC()

