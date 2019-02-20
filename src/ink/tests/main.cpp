#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(bitmap);
    Y_UTEST(areas);
    Y_UTEST(pixmaps);
    Y_UTEST(image);
    Y_UTEST(pixmap);
    Y_UTEST(ops);
    Y_UTEST(blur);
    Y_UTEST(hist);
    Y_UTEST(blob);
    Y_UTEST(edges);
    Y_UTEST(mask);
    Y_UTEST(draw);
    Y_UTEST(iso);
    Y_UTEST(kr);
    Y_UTEST(crop);
}
Y_UTEST_EXEC()

