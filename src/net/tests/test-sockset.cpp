#include "y/net/socket/set.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;


Y_UTEST(sockset)
{
    std::cerr << "socket_set::capacity = " << net::socket_set::capacity << std::endl;
    std::cerr << "sizeof(socket_set)   = " << sizeof(net::socket_set) << std::endl;
    net::socket_set sockset;
    std::cerr << "sockset.allocated    = " << sockset.allocated << std::endl;

    net::bsd_socket tcp_v4(net::tcp,net::v4);
    net::bsd_socket tcp_v6(net::tcp,net::v6);
    net::bsd_socket udp_v4(net::udp,net::v4);
    net::bsd_socket udp_v6(net::udp,net::v6);

    net::bsd_socket *arr[4] = { &tcp_v4 , &tcp_v6, &udp_v4, &udp_v6 };

    for(size_t iter=0;iter<128;++iter)
    {
        alea.shuffle(arr,sizeof(arr)/sizeof(arr[0]));
        for(size_t i=0;i<sizeof(arr)/sizeof(arr[0]);++i)
        {
            sockset.insert(*arr[i]);
        }
        sockset.free();
    }

}
Y_UTEST_DONE()

