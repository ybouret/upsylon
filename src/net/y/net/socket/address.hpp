//! \file
#ifndef Y_NET_SOCKET_ADDRESS_INCLUDED
#define Y_NET_SOCKET_ADDRESS_INCLUDED 1

#include "y/net/types.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //! Human Readable Buffer
        //______________________________________________________________________
        template <size_t N> class hrbuff
        {
        public:
            //! setup
            inline  hrbuff() throw()    { clear(); }
            //! cleanup
            inline ~hrbuff() throw()    { clear(); }
            //! copy
            inline  hrbuff(const hrbuff &other) throw() { for(size_t i=0;i<N;++i) ch[i]=other.ch[i]; }
            //! clear
            inline void clear() throw() { for(size_t i=0;i<N;++i) ch[i] = 0; }

            char ch[N]; //!< workspace
        private:
            Y_DISABLE_ASSIGN(hrbuff);
        };

        //______________________________________________________________________
        //
        //! default address initializer
        //______________________________________________________________________
        enum ip_addr_value
        {
            ip_addr_none,     //!< error like
            ip_addr_any,      //!< any interface
            ip_addr_loopback  //!< loopback interface
        };

        

        //______________________________________________________________________
        //
        //! socket address interface[4|6]
        //______________________________________________________________________
        class socket_address : public memory::rw_buffer, public net_object
        {
        public:
            static const char port_separator = '@';  //!< to parse address
            template <ip_version V> class format;    //!< version dependent format

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual            ~socket_address() throw();      //!< destructor
            virtual const char *text()      const throw() = 0; //!< format internal buffer
            virtual const char *className() const throw() = 0; //!< for information
            virtual unsigned    family()    const throw() = 0; //!< for information/naming
            virtual ip_version  version()   const throw() = 0; //!< for information

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            //! display
            friend inline std::ostream & operator<<(std::ostream &os, const socket_address &i )
            {
                return (os << i.text() << port_separator << bswp(i.port) );
            }

            void resolve( const string &xname );      //!< using network::resolve
            void resolve( const char   *xname );      //!< using network::resolve
            void bind(    socket_type &sock ) const;  //!< bind address to socket
            void connect( socket_type &sock ) const;  //!< connect to address via socket
            net16_t & port; //!< network byte order port

            sockaddr       *get_sa() throw();              //!< static_cast<sockaddr *>( rw() )
            const sockaddr *get_sa() const throw();        //!< static_cast<const sockaddr *>( ro() )
            sa_length_t     get_sa_length() const throw(); //!< static_cast<sa_length_t>( length() )


            
        protected:
            //! setup
            socket_address(void          *data,
                           const unsigned port_offset) throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(socket_address);
        };

        //______________________________________________________________________
        //
        //! IPv6 format
        //______________________________________________________________________
        template <> class socket_address:: format<v6>
        {
        public:
            //__________________________________________________________________
            //
            // typedef and metrics
            //__________________________________________________________________
            static  const char     class_name[];                           //!< "ipv6"
            typedef sockaddr_in6   type;                                   //!< net API
            static  const unsigned port_offset = offsetof(type,sin6_port); //!< locate port

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            type      sa;   //!< low-level data
            net128_t &addr; //!< binary mapping

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual ~format() throw();           //!< destructor
            format(const format &other) throw(); //!< copy

            void        _( const ip_addr_value value ) throw(); //!< set to a specific value
            const char *hr() const throw();                     //!< format internal buffer
            unsigned    pf() const throw();                     //!< return sin6_family

        protected:
            explicit format(const ip_addr_value value) throw(); //!< setup

        private:
            Y_DISABLE_ASSIGN(format);
            mutable hrbuff<64> hrb; //! strlen(XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX:XXXX) <= 49 => 52
        };

        //______________________________________________________________________
        //
        //! IPv4 format
        //______________________________________________________________________
        template <> class socket_address:: format<v4>
        {
        public:
            //__________________________________________________________________
            //
            // types and metrics
            //__________________________________________________________________
            static  const char      class_name[]; //!< "ipv4"
            typedef sockaddr_in     type;         //!< net API
            static  const unsigned  port_offset = offsetof(type,sin_port); //!< locate port

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            type     sa;   //!< low-level data
            net32_t &addr; //!< binary mapping

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual ~format() throw();           //!< destructor
            format(const format &other) throw(); //!< copy

            void     _( const ip_addr_value value ) throw(); //!< set to a specific value
            const char *hr() const throw();                  //!< format internal buffer
            unsigned    pf() const throw();                  //!< return sin_family

        protected:
            explicit format(const ip_addr_value value) throw(); //!< setup

        private:
            Y_DISABLE_ASSIGN(format);
            mutable hrbuff<16> hrb; //! strlen(XXX.XXX.XXX.XXX)<= 15 => 16
        };

        //! wrapper for initialization
#define Y_NET_SOCKET_ADDR(VALUE) format_type(VALUE), socket_address( &(this->sa), __port)

        //______________________________________________________________________
        //
        //! generic socket address
        //______________________________________________________________________
        template <ip_version V>
        class socket_addr : public socket_address::format<V>, public socket_address
        {
        public:
            //__________________________________________________________________
            //
            // types and metrics
            //__________________________________________________________________
            typedef socket_address::format<V>  format_type; //!< alias
            typedef typename format_type::type sa_type;     //!< alias
            static const size_t                sa_size = sizeof(sa_type); //!< alias
            static const size_t                __port  = format_type::port_offset; //!< alias


            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            inline virtual const void *ro()        const throw() { return &(this->sa); }       //!< rw_buffer interface
            inline virtual size_t      length()    const throw() { return sa_size;     }       //!< rw_buffer interface
            inline virtual const char *text()      const throw() { return this->hr(); }        //!< socket_address interface
            inline virtual const char *className() const throw() { return this->class_name; }  //!< socket_address/serial interface
            inline virtual unsigned    family()    const throw() { return this->pf(); }        //!< socket_address interface
            inline virtual ip_version  version()   const throw() { return V;          }        //!< the version
            inline virtual            ~socket_addr()     throw() {}                            //!< destructor

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________


            //! setup by value(s)
            inline socket_addr(const ip_addr_value value     = ip_addr_none,
                               const uint16_t      user_port = 0 ) throw() :
            Y_NET_SOCKET_ADDR(value)
            {
                port = bswp(user_port);
            }

            inline socket_addr( const string &xname  ) :         Y_NET_SOCKET_ADDR(ip_addr_none) { resolve(xname); } //!< setup by resolve
            inline socket_addr( const char   *xname  ) :         Y_NET_SOCKET_ADDR(ip_addr_none) { resolve(xname); } //!< setup by resolve
            inline socket_addr( const socket_addr &i ) throw() : Y_NET_SOCKET_ADDR(i) {}                             //!< copy

            //! assign
            inline socket_addr & assign( const socket_addr &other ) throw()
            {
                memmove( &(this->sa), &(other.sa), sizeof(sa_type) );
                return *this;
            }

            socket_addr & operator=( const socket_addr &other ) throw() { return assign(other); }                //!< assign
            socket_addr & operator=( const string &xname ) { const socket_addr tmp(xname); return assign(tmp); } //!< resolve/assign
            socket_addr & operator=( const char   *xname ) { const socket_addr tmp(xname); return assign(tmp); } //!< resolve/assign
        };

        typedef socket_addr<v4> ipv4; //!< alias for level 4
        typedef socket_addr<v6> ipv6; //!< alias for level 6


    }
}

#endif

