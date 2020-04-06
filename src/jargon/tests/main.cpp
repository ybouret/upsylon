
#include "y/utest/driver.hpp"

Y_UTEST_INIT(32)
{
    Y_UTEST(types);
    Y_UTEST(module);
    Y_UTEST(source);
    Y_UTEST(pattern);
    Y_UTEST(posix);
    Y_UTEST(regexp);
    Y_UTEST(regcom);
    Y_UTEST(scanner);
    Y_UTEST(lexer);
    Y_UTEST(matching);
    
    Y_UTEST(xnode);
    Y_UTEST(grammar);
    Y_UTEST(parser);
    Y_UTEST(expr);
}
Y_UTEST_EXEC()
