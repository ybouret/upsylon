#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(scatter);
    Y_UTEST(scanner);
    Y_UTEST(lexer);
    Y_UTEST(editor);
}
Y_UTEST_EXEC()
