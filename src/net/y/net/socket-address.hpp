// !\file
#ifndef Y_NET_SOCKET_ADDRESS_INCLUDED
#define Y_NET_SOCKET_ADDRESS_INCLUDED 1

#include "y/net/types.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace net
    {

        //! Human Readable Buffer
        template <size_t N> class hrbuff
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

        //! socket address interface[4|6]
        class socket_address : public memory::rw_buffer
        {
        public:
            virtual ~socket_address() throw();
            
            net16_t & port;

            virtual const char *text()      const throw() = 0; //!< format internal buffer
            virtual const char *className() const throw() = 0; //!< for information
            virtual unsigned    family()    const throw() = 0; //!< for information/naming

            friend inline std::ostream & operator<<(std::ostream &os, const socket_address &i )
            {
                return (os << i.text());
            }

            template <ip_version V> class format; //!< version dependent format






        protected:
            //! setup
            socket_address(void          *data,
                           const unsigned port_offset) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(socket_address);
        };


        //! IPv6 format
        template <> class socket_address:: format<v6>
        {
        public:
            static  const char     class_name[]; //!< "ipv6"
            typedef sockaddr_in6   type;         //!< net API
            static  const unsigned port_offset = offsetof(type,sin6_port); //!< locate port

            type      sa;   //!< low-level data
            net128_t &addr; //!< binary mapping

            virtual ~format() throw();           //!< destructor
            format(const format &other) throw(); //!< copy

            void     _( const ip_addr_value value ) throw(); //!< set to a specific value
            const char *hr() const throw();                  //!< format internal buffer
            unsigned    pf() const throw();                  //!< return sin_family

        protected:
            explicit format(const ip_addr_value value) throw(); //!< setup

        private:
            Y_DISABLE_ASSIGN(format);
            mutable hrbuff<64> hrb;
        };

        //! IPv4 format
        template <> class socket_address:: format<v4>
        {
        public:
            static  const char      class_name[]; //!< "ipv4"
            typedef sockaddr_in     type;         //!< net API
            static  const unsigned  port_offset = offsetof(type,sin_port); //!< locate port

            type     sa;   //!< low-level data
            net32_t &addr; //!< binary mapping

            virtual ~format() throw();           //!< destructor
            format(const format &other) throw(); //!< copy

            void     _( const ip_addr_value value ) throw(); //!< set to a specific value
            const char *hr() const throw();                  //!< format internal buffer
            unsigned    pf() const throw();                  //!< return sin6_family

        protected:
            explicit format(const ip_addr_value value) throw(); //!< setup

        private:
            Y_DISABLE_ASSIGN(format);
            mutable hrbuff<16> hrb;
        };


        template <ip_version V>
        class socket_addr : public socket_address::format<V>, public socket_address
        {
        public:
            typedef socket_address::format<V> format_type;
            static const size_t             __port = format_type::port_offset;

            inline virtual ~socket_addr() throw() {}

            inline socket_addr( const ip_addr_value value = ip_addr_none, uint16_t user_port = 0 ) throw() :
            format_type( value ), socket_address( &(this->sa), __port)
            {
                port = bswp(user_port);
            }

            inline socket_addr( const socket_addr &i ) throw() : format_type(i), socket_address( &(this->sa), __port) {}
            
            inline virtual const void *ro()        const throw() { return &(this->sa); }
            inline virtual size_t      length()    const throw() { return sizeof(typename format_type::type); }
            inline virtual const char *text()      const throw() { return this->hr(); }
            inline virtual const char *className() const throw() { return this->class_name; }
            inline virtual unsigned    family()    const throw() { return this->pf(); }

        private:
            Y_DISABLE_ASSIGN(socket_addr);
        };

        typedef socket_addr<v4> ipv4;
        typedef socket_addr<v6> ipv6;
        
    }
}

#endif

