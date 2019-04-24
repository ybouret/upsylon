#include "y/net/socket/addr-ex.hpp"

namespace upsylon
{
    namespace net
    {
        socket_addr_ex:: ~socket_addr_ex() throw()
        {
            memset(wksp,0,sizeof(wksp));
            addr=0;
        }

        void  socket_addr_ex::setup( const socket_address &ip ) throw()
        {
            assert(NULL==addr);
            memset(wksp,0,sizeof(wksp));
            switch(ip.version())
            {
                case v4:
                {
                    ipv4  *a = new( memory::io::__force<ipv4>(wksp) ) ipv4();
                    addr = a;
                    assert( a->length() == ipv4::sa_size );
                    assert( ip.length() == ipv4::sa_size );
                    memcpy( a->rw(), ip.ro(), ipv4::sa_size );
                } break;

                case v6:
                {
                    ipv6  *a = new( memory::io::__force<ipv6>(wksp) ) ipv6(); assert( a->length() == ipv6::sa_size );
                    addr = a;
                    assert( a->length() == ipv6::sa_size );
                    assert( ip.length() == ipv6::sa_size );
                    memcpy( a->rw(), ip.ro(), ipv6::sa_size );
                }   break;

            }
        }


        socket_addr_ex:: socket_addr_ex(const socket_address &ip) throw() :
        wksp(),
        addr(NULL)
        {
            setup(ip);
        }


        void socket_addr_ex:: setup( const string &xname, const ip_version version)
        {
            switch (version) {
                case v4:
                {
                    const ipv4 tmp(xname);
                    setup(tmp);
                } break;

                case v6:
                {
                    const ipv6 tmp(xname);
                    setup(tmp);
                } break;
            }
        }

        socket_addr_ex:: socket_addr_ex( const string &xname, const ip_version version):
        wksp(),
        addr(NULL)
        {
            setup(xname,version);
        }

        socket_addr_ex:: socket_addr_ex( const char *xname, const ip_version version) :
        wksp(),
        addr(NULL)
        {
            const string _(xname);
            setup(_,version);
        }


        socket_addr_ex:: socket_addr_ex( const ip_addr_value value, const ip_version version, const uint16_t user_port) throw() :
        wksp(),
        addr(NULL)
        {
            memset(wksp,0,sizeof(wksp));
            switch(version)
            {
                case v4:
                {
                    ipv4  *a = new( memory::io::__force<ipv4>(wksp) ) ipv4(value,user_port);
                    addr = a;
                } break;

                case v6:
                {
                    ipv6  *a = new( memory::io::__force<ipv6>(wksp) ) ipv6(value,user_port);
                    addr = a;
                }   break;

            }
        }

        socket_address & socket_addr_ex:: operator*() throw()
        {
            assert(addr);
            return *addr;
        }

        const socket_address & socket_addr_ex:: operator*() const throw()
        {
            assert(addr);
            return *addr;
        }


        socket_address * socket_addr_ex:: operator->() throw()
        {
            assert(addr);
            return addr;
        }

        const socket_address * socket_addr_ex:: operator->() const throw()
        {
            assert(addr);
            return addr;
        }



    }

}
