#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(network);
    Y_UTEST(bsd);
    Y_UTEST(addr);    
}
Y_UTEST_EXEC()

