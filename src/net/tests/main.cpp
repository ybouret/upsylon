#include "y/utest/driver.hpp"

Y_UTEST_INIT(16)
{
    Y_UTEST(network);
    Y_UTEST(bsd);
    Y_UTEST(addr);
    Y_UTEST(resolve);
    Y_UTEST(udp_client);
    Y_UTEST(udp_server);
    Y_UTEST(tcp_client);
    Y_UTEST(tcp_server);
    Y_UTEST(info);
    Y_UTEST(sockset);
    Y_UTEST(io);
}
Y_UTEST_EXEC()

