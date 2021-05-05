
#include "y/utest/driver.hpp"

Y_UTEST_INIT(8)
{
    Y_UTEST(init);
    Y_UTEST(init_env);
    Y_UTEST(ring);
    Y_UTEST(coll);
    Y_UTEST(xmpi);
    Y_UTEST(spade);
    Y_UTEST(com);
}
Y_UTEST_EXEC()
