 #include "y/code/property.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(ppty)
{
    std::cerr << "int ppty" << std::endl;

    int my_int = 5;

    property<int> I( my_int, "I", false );

    std::cerr << "read ppty" << std::endl;
    int j = I;
    std::cerr << "j=" << j << std::endl;

    std::cerr << "bool ppty" << std::endl;

    bool my_sending = true;

    property<bool> sending( my_sending, "sending", true );

    std::cerr << "sending=" << ( sending ?  "true" : "false" ) << std::endl;
    sending = false;
    std::cerr << "sending=" << ( sending ?  "true" : "false" ) << std::endl;

    try
    {
        std::cerr << "write ppty" << std::endl;
        I = 10;
    }
    catch(const exception &e )
    {
        std::cerr << "** Exception:" << std::endl;
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unhandled exception" << std::endl;
    }
}
Y_UTEST_DONE()

