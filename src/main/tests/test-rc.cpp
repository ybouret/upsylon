
#include "y/ios/rc/io.hpp"
#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(rc)
{
    {
        ios::ocstream fp("rc.bin");
        fp << "Hello, World!\n";
    }

    {
        ios::rc::writer rc("rc.bin");
        rc.append_data("my special code","code/main");
        rc.append_data("another code","code/second");

        rc.finish();
    }
}
Y_UTEST_DONE()

