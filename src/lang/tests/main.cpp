#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(token);
    Y_UTEST(source);
    Y_UTEST(pattern);
    Y_UTEST(posix);
    Y_UTEST(regex);
    Y_UTEST(scanner);    
}
Y_UTEST_EXEC()
