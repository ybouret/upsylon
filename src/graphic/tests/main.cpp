#include "y/utest/driver.hpp"

Y_UTEST_INIT(32)
{
    Y_UTEST(types);
    Y_UTEST(area);
    Y_UTEST(bitmap);
    Y_UTEST(pixmap);
    Y_UTEST(stack);
    Y_UTEST(parallel);
    Y_UTEST(gradient);
    Y_UTEST(png);
    Y_UTEST(jpeg);
    Y_UTEST(tiff);
    Y_UTEST(image);
    Y_UTEST(ramp);
    Y_UTEST(img2ramp);
    Y_UTEST(imgrad);
    Y_UTEST(filter);
    Y_UTEST(grads);
    Y_UTEST(blobs);
    Y_UTEST(3x3);
    Y_UTEST(blur);
    Y_UTEST(edges);
    Y_UTEST(channels);
    Y_UTEST(draw);
}
Y_UTEST_EXEC()

