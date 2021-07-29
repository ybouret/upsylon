#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(species);
    Y_UTEST(library);
    Y_UTEST(actor);
    Y_UTEST(actors);
    Y_UTEST(types);
    Y_UTEST(eq);
}
Y_UTEST_EXEC()
