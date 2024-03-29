#include "y/net/socket/set.hpp"
#include "y/net/socket/delay.hpp"
#include "y/memory/zblock.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/global.hpp"

using namespace upsylon;

namespace
{
    static inline
    void show( const net::socket_delay &d )
    {
        std::cerr << "wait_for=" << d.wait_for() << std::endl;
    }

    static inline
    void check_different(net::bsd_socket **arr, const size_t num )
    {
        assert(arr);
        assert(num>0);
        
        for(size_t i=0;i<num;++i)
        {
            for(size_t j=0;j<num;++j)
            {
                if(i!=j)
                {
                    Y_ASSERT(arr[i]->sock!=arr[j]->sock);
                }
                else
                {
                    Y_ASSERT(arr[i]->sock==arr[j]->sock);
                }
            }
        }
    }

    static inline
    void fill( net::bsd_socket **arr, size_t &num, net::bsd_socket &s )
    {
        arr[num++] = &s;
    }

    static inline
    void insert_all(net::socket_set  &sockset,
                    net::bsd_socket **arr,
                    const size_t      num )
    {
        for(size_t i=0;i<num;++i)
        {
            sockset.insert(*arr[i]);
        }
    }

    static inline
    void remove_all(net::socket_set  &sockset,
                    net::bsd_socket **arr,
                    const size_t      num )
    {
        for(size_t i=0;i<num;++i)
        {
            sockset.remove(*arr[i]);
        }
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

    zblock<net::bsd_socket *,memory::global> blk(4);

    {
        net::bsd_socket **arr = &blk[1];
        size_t            num = 0;

        fill(arr, num, tcp_v4);
        fill(arr, num, tcp_v6);
        fill(arr, num, udp_v4);
        fill(arr, num, udp_v6);

        check_different(arr,num);

        std::cerr << "<i/o for sockset>" << std::endl;
        for(size_t iter=0;iter<128;++iter)
        {
            std::cerr << ".";
            alea.shuffle(arr,num);
            check_different(arr,num);
            insert_all(sockset,arr,num);
            Y_ASSERT(num==sockset.size);

            alea.shuffle(arr,num);
            check_different(arr,num);
            remove_all(sockset,arr,num);
            Y_ASSERT(sockset.size==0);

            alea.shuffle(arr,num);
            check_different(arr,num);
            insert_all(sockset,arr,num);
            Y_ASSERT(num==sockset.size);
            sockset.free();
            Y_ASSERT(sockset.size==0);
        }
        std::cerr << std::endl << "<i/o for sockset/>" << std::endl;


    }

    {
        net::socket_delay d1, d2=0, d3=1.01;
        show(d1);
        show(d2);
        show(d3);

        sockset.free();

        net::bsd_socket *arr[4] = { &tcp_v4 , &tcp_v6, &udp_v4, &udp_v6 };
        const char      *id[4]  = { "tcp_v4", "tcp_v6", "udp_v4", "udp_v6" };
        const size_t     num    = sizeof(arr)/sizeof(arr[0]);
        for(size_t i=0;i<num;++i)
        {
            sockset.insert(*arr[i]);
        }
        const size_t ans = sockset.incoming(d2);
        std::cerr << "probe=" << ans << std::endl;

        for(size_t i=0;i<num;++i)
        {
            const net::bsd_socket &s = *arr[i];
            std::cerr << id[i] << ":";
            if( sockset.is_exception(s) ) std::cerr << "x"; else std::cerr << "-";
            if( sockset.is_readable(s)  ) std::cerr << "r"; else std::cerr << "-";
            //if( sockset.is_writable(s)  ) std::cerr << "w"; else std::cerr << "-";
            std::cerr << std::endl;
        }
    }

}
Y_UTEST_DONE()

