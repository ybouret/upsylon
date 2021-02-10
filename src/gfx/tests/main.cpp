#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(types);
    Y_UTEST(area);
    Y_UTEST(tile);
    Y_UTEST(tiles);
    Y_UTEST(engine);
    Y_UTEST(pixels);
    Y_UTEST(bitmap);
    Y_UTEST(pixmap);
    Y_UTEST(broker);
}
Y_UTEST_EXEC()
