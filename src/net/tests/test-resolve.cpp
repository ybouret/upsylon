
#include "y/net/net.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(resolve)
{
    network &nw = network::instance();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string name = argv[1];
        std::cerr << "resolving [" << name << "]" << std::endl;
        net::ipv4    ip4;
        net::ipv6    ip6;
        nw.resolve(ip4,name);
        std::cerr << "          |_" << ip4 << std::endl;

        try
        {
            nw.resolve(ip6,name);
            std::cerr << "          |_" << ip6 << std::endl;
        }
        catch(const exception &e)
        {
            std::cerr << e.when() << " : " << e.what() << std::endl;
            continue;
        }
        catch(...)
        {
            std::cerr << "unhandled exception..." << std::endl;
            throw;
        }
        
    }
}
Y_UTEST_DONE()

