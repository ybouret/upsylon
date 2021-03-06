
#include "y/net/socket/address.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(resolve)
{

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string name = argv[1];
        std::cerr << "resolving [" << name << "]" << std::endl;
        net::ipv4    ip4;
        net::ipv6    ip6;
        ip4 = name;
        std::cerr << "          |_" << ip4.className() << "=" << ip4 << "@" << net::bswp(ip4.port) <<std::endl;

        try
        {
            ip6 = name;
            std::cerr << "          |_" << ip6.className() << "=" << ip6 << "@" << net::bswp(ip6.port) <<std::endl;

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

