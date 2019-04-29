#include "y/net/socket/set.hpp"
#include "y/net/socket/delay.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static inline
    void show( const net::socket_delay &d )
    {
        std::cerr << "usr=" << d.usr.tv_sec  << "+1e6*" << d.usr.tv_usec << std::endl;
    }
}

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
    const size_t     num    = sizeof(arr)/sizeof(arr[0]);

    std::cerr << "<i/o for sockset>" << std::endl;
    for(size_t iter=0;iter<128;++iter)
    {
        std::cerr << ".";
        alea.shuffle(arr,num);
        for(size_t i=0;i<num;++i)
        {
            sockset.insert(*arr[i]);
        }
        Y_ASSERT(num==sockset.size);

        alea.shuffle(arr,num);
        for(size_t i=0;i<num;++i)
        {
            sockset.remove(*arr[i]);
        }
        Y_ASSERT(sockset.size==0);

        alea.shuffle(arr,num);
        for(size_t i=0;i<num;++i)
        {
            sockset.insert(*arr[i]);
        }
        Y_ASSERT(num==sockset.size);
        sockset.free();
        Y_ASSERT(sockset.size==0);

    }
    std::cerr << "<i/o for sockset/>" << std::endl;

    net::socket_delay d1, d2=0, d3=1.01;
    show(d1);
    show(d2);
    show(d3);

}
Y_UTEST_DONE()

