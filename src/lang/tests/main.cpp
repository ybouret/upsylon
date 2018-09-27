#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(token);
    Y_UTEST(source);
    Y_UTEST(pattern);
    Y_UTEST(posix);
    Y_UTEST(regex);
    Y_UTEST(matching);
    Y_UTEST(scanner);
    Y_UTEST(lex);
    Y_UTEST(grammar);
    Y_UTEST(parser);
    Y_UTEST(eval);
    Y_UTEST(json);
    Y_UTEST(dyn);

}
Y_UTEST_EXEC()
