#include "y/utest/driver.hpp"

Y_UTEST_INIT(32)
{
    Y_UTEST(platform);
    Y_UTEST(types);
    Y_UTEST(lockable);
    Y_UTEST(swap);
    Y_UTEST(zset);
    Y_UTEST(at_exit);
    Y_UTEST(sys);
    Y_UTEST(bits);
    Y_UTEST(chunk);
    Y_UTEST(code);
    Y_UTEST(alloc);
    Y_UTEST(core);
    Y_UTEST(arena);
    Y_UTEST(blocks);
    Y_UTEST(slab);
    Y_UTEST(primality);
    Y_UTEST(object0);
    Y_UTEST(objectY);
    Y_UTEST(slice);
    Y_UTEST(carver);
    Y_UTEST(pooled);
    Y_UTEST(buffers);
    Y_UTEST(string);
    Y_UTEST(traits);
    Y_UTEST(threads);

}
Y_UTEST_EXEC()

