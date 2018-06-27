#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(platform);
    Y_UTEST(types);
    Y_UTEST(lockable);
    Y_UTEST(swap);
    Y_UTEST(at_exit);
    Y_UTEST(sys);
    Y_UTEST(code);
    Y_UTEST(kChunk);

}
Y_UTEST_EXEC()

