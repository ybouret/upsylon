// !\file
#ifndef Y_NET_IP_ADDR_INCLUDED
#define Y_NET_IP_ADDR_INCLUDED 1

#include "y/net/types.hpp"
//#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace net
    {

        enum ip_addr_value
        {
            ip_addr_none,
            ip_addr_any
        };


        class ip_address
        {
        public:
            virtual ~ip_address() throw();

            net16_t & port;

        protected:
            ip_address(void          *data,
                       const unsigned port_offset) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_address);
        };

        template <ip_version V>
        class ip_data;

        template <> class ip_data<v4>
        {
        public:

            typedef sockaddr_in     type;
            static  const unsigned  port_offset = offsetof(type,sin_port);

            type sa;

            virtual ~ip_data() throw();

        protected:
            explicit ip_data() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_data);
        };

        template <> class ip_data<v6>
        {
        public:
            typedef sockaddr_in6   type;
            static  const unsigned port_offset = offsetof(type,sin6_port);
            
            type sa;

            virtual ~ip_data() throw();

        protected:
            explicit ip_data() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_data);
        };

        template <ip_version V>
        class ip_addr : public ip_data<V>, public ip_address
        {
        public:
            typedef ip_data<V> ip_base;

            inline virtual ~ip_addr() throw() {}

            inline ip_addr( uint16_t user_port ) throw() :
            ip_base(),
            ip_addr( & this->sa, ip_base::port_offset )
            {
                port = swap_nbo(user_port);
            }


        private:

        };


    }
}

#endif

