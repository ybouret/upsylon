
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
        rc.append_data("code/first","this is the first part");
        rc.append_data("code/second","this is the second part");

        rc.finalize();

        
    }
}
Y_UTEST_DONE()

