#include "y/net/udp/client.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(udp_client)
{
    if(argc<=3) throw exception("%s: dest[:port] version msg",argv[0]);

    const string xname   = argv[1];
    const string version = argv[2];
    const string msg     = argv[3];

    

    

}
Y_UTEST_DONE()


