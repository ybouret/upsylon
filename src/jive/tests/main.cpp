#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(scatter);
    Y_UTEST(scanner);
    Y_UTEST(lexer);
    Y_UTEST(editor);
    Y_UTEST(editor64);
    Y_UTEST(ed_snake);
    Y_UTEST(axiom);    
    Y_UTEST(grammar);
    Y_UTEST(types);
}
Y_UTEST_EXEC()
