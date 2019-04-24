// !\file
#ifndef Y_NET_IP_ADDR_INCLUDED
#define Y_NET_IP_ADDR_INCLUDED 1

#include "y/net/types.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace net
    {

        template <size_t N>
        class hrbuff
        {
        public:
            inline  hrbuff() throw()    { clear(); }
            inline ~hrbuff() throw()    { clear(); }
            inline  hrbuff(const hrbuff &other) throw() { for(size_t i=0;i<N;++i) ch[i]=other.ch[i]; }
            inline void clear() throw() { for(size_t i=0;i<N;++i) ch[i] = 0; }

            char ch[N];
        private:
            Y_DISABLE_ASSIGN(hrbuff);
        };

        enum ip_addr_value
        {
            ip_addr_none,
            ip_addr_any,
            ip_addr_loopback
        };


        class ip_address : public memory::rw_buffer
        {
        public:
            virtual ~ip_address() throw();
            
            net16_t & port;

            virtual const char *text() const throw() = 0;

            friend inline std::ostream & operator<<(std::ostream &os, const ip_address &i )
            {
                return (os << i.text());
            }



        protected:
            ip_address(void          *data,
                       const unsigned port_offset) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_address);
        };

        template <ip_version V>
        class ip_data;

       

        template <> class ip_data<v6>
        {
        public:
            typedef sockaddr_in6   type;
            static  const unsigned port_offset = offsetof(type,sin6_port);
            
            type      sa;
            net128_t &addr;
            
            virtual ~ip_data() throw();

            void     _( const ip_addr_value value ) throw();
            const char *hr() const throw(); //!<

        protected:
            explicit ip_data(const ip_addr_value value) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_data);
            mutable hrbuff<64> hrb;
        };

        template <> class ip_data<v4>
        {
        public:
            
            typedef sockaddr_in     type;
            static  const unsigned  port_offset = offsetof(type,sin_port);
            
            type     sa;
            net32_t &addr;
            virtual ~ip_data() throw();
            
            void     _( const ip_addr_value value ) throw();
            const char *hr() const throw(); //!<

        protected:
            explicit ip_data(const ip_addr_value value) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_data);
            mutable hrbuff<16> hrb;
        };
        
        template <ip_version V>
        class ip_addr : public ip_data<V>, public ip_address
        {
        public:

            inline virtual ~ip_addr() throw() {}

            inline ip_addr( const ip_addr_value value, uint16_t user_port ) throw() :
            ip_data<V>( value ),
            ip_address( & this->sa, ip_data<V>::port_offset )
            {
                port = bswp(user_port);
            }
            
            virtual const void *ro()     const throw() { return &(this->sa); }
            virtual size_t      length() const throw() { return sizeof(typename ip_data<V>::type); }
            virtual const char *text()   const throw() { return this->hr(); }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_addr);
        };

        typedef ip_addr<v4> ipv4;
        typedef ip_addr<v6> ipv6;
        
    }
}

#endif

