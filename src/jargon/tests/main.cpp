
#include "y/utest/driver.hpp"

Y_UTEST_INIT(8)
{
    Y_UTEST(types);    
    Y_UTEST(xnode);
    Y_UTEST(grammar);
    Y_UTEST(parser);
}
Y_UTEST_EXEC()
