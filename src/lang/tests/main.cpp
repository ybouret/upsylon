#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
   
    Y_UTEST(scanner);
    Y_UTEST(lex);
    Y_UTEST(grammar);
    Y_UTEST(parser);
    Y_UTEST(eval);
    Y_UTEST(json);
    Y_UTEST(dyn);
    Y_UTEST(gen);
    Y_UTEST(hash);
}
Y_UTEST_EXEC()
