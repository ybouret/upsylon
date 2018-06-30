#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(platform);
    Y_UTEST(types);
    Y_UTEST(lockable);
    Y_UTEST(swap);
    Y_UTEST(at_exit);
    Y_UTEST(sys);
    Y_UTEST(bits);
    Y_UTEST(chunk);
    Y_UTEST(code);
    Y_UTEST(alloc);
    Y_UTEST(core);
    Y_UTEST(arena);
    Y_UTEST(blocks);
    Y_UTEST(primality);
    Y_UTEST(object0);
    Y_UTEST(objectY);

}
Y_UTEST_EXEC()

