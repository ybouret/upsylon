
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

    {
        ios::rc::loader rc("rc.bin");
        string s1 = rc.load_string("code/first");  std::cerr << "s1=" << s1 << "/" << s1.size() << std::endl;
        string s2 = rc.load_string("code/second"); std::cerr << "s1=" << s2 << "/" << s2.size() << std::endl;

        
    }
}
Y_UTEST_DONE()

