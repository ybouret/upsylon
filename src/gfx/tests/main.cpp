#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(area);
    Y_UTEST(bitmap);
    Y_UTEST(pixmaps);
    Y_UTEST(image);
    Y_UTEST(options);
    Y_UTEST(stack);
    Y_UTEST(tiles);
    Y_UTEST(tess);
    Y_UTEST(ops);
    Y_UTEST(grads);
}
Y_UTEST_EXEC()
