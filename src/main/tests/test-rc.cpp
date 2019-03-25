
#include "y/ios/rc/io.hpp"
#include "y/ios/ocstream.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;

Y_UTEST(rc)
{
    {
        ios::ocstream fp("rc.bin");
        fp << "Hello, World!\n";
    }

    std::cerr << "-- writer" << std::endl;
    {
        ios::rc::writer rc("rc.bin",true);
        rc.append_data("code/first","this is the first part");
        rc.append_data("code/second","this is the second part");

        if(argc>1)
        {
            rc.append_file("file",argv[1]);
        }

        rc.finalize();
    }


    std::cerr << "-- loader" << std::endl;
    {
        ios::rc::loader rc("rc.bin",true);
        string s1 = rc.load_string("code/first");  std::cerr << "s1=" << s1 << "/" << s1.size() << std::endl;
        string s2 = rc.load_string("code/second"); std::cerr << "s1=" << s2 << "/" << s2.size() << std::endl;

        auto_ptr<ios::istream> p1 = rc.load_stream("code/first");
        std::cerr << "displaying stream:[";
        {
            char C;
            while(p1->query(C))
            {
                std::cerr << C;
            }
        }
        std::cerr << "]" << std::endl;
        
    }
}
Y_UTEST_DONE()

