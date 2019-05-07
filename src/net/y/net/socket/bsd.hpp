//! \file
#ifndef Y_NET_BSD_SOCKET_INCLUDED
#define Y_NET_BSD_SOCKET_INCLUDED 1

#include "y/net/net.hpp"
#include "y/codec/base64-name.hpp"

namespace upsylon
{
    namespace net
    {
        //! shutdown socket mnode
        enum shutdown_type
        {
            sd_send, //!< stop sending
            sd_recv, //!< stop receiving
            sd_both  //!< stop all
        };

#if defined(Y_BSD)
        typedef int socket_boolean; //!< system boolean type
#endif

#if defined(Y_WIN)
        typedef BOOL socket_boolean; //!< system boolean type
#endif
        extern const socket_boolean socket_true;  //!< opaque TRUE value
        extern const socket_boolean socket_false; //!< opaque FALSE value

        
        //! low level socket API
        class bsd_socket : public net_object
        {
        public:
            typedef base64_name<socket_id_t> name_type; //!< alias for named socket

            //__________________________________________________________________
            //
            // open and close
            //__________________________________________________________________
            virtual ~bsd_socket() throw();                  //!< destructor
            explicit bsd_socket(const ip_protocol protocol, //
                                const ip_version  version); //!< setup

            void shutdown(const shutdown_type) throw(); //!< shutdown
            static const char *sd_text(const shutdown_type) throw(); //!< for shutdown info

            //__________________________________________________________________
            //
            // options names for debug
            //__________________________________________________________________
            static const char *sockopt_level(const int level)  throw(); //!< socket option level text
            static const char *sockopt_name(const int optname) throw(); //!< socket option name
            
            //__________________________________________________________________
            //
            // set options
            //__________________________________________________________________
            void blocking(const bool value);    //!< set blocking value
            void setopt(const int      level,   //
                        const int      optname, //
                        const void    *optval,  //
                        const unsigned optlen); //!< wrapper to setsocketopt

            //! wrapper to integral values
            template <typename T> inline void setopt( const int level, const int optname, const T value )
            {
                setopt(level,optname, &value, sizeof(T) );
            }

            void on(  const int level, const int optname ); //!< turn flag on,  using system boolean
            void off( const int level, const int optname ); //!< turn flag off, using system boolean
            void sndbuf(const unsigned bytes);              //!< modify SO_SNDBUF
            void rcvbuf(const unsigned bytes);              //!< modify SO_RCVBUF


            //__________________________________________________________________
            //
            // get options
            //__________________________________________________________________
            //! wrapper to getsockopt
            void getopt(const int      level,
                        const int      optname,
                        void          *optval,
                        const unsigned optlen) const;

            //! for integral types
            template <typename T> inline T getopt(const int level, const int optname ) const
            {
                uint64_t optval[ Y_U64_FOR_ITEM(T) ] = { 0 };
                getopt(level,optname,optval,sizeof(T));
                return *static_cast<T*>(addr2addr(optval));
            }

            bool test(const int level, const int optname) const; //!< boolean value
            int  sndbuf() const; //!< current SO_SNDBUF
            int  rcvbuf() const; //!< current SO_RCVBUF

            //__________________________________________________________________
            //
            // handling
            //__________________________________________________________________
            const size_t  & key() const throw(); //!< get key for set

        protected:
            explicit     bsd_socket( const socket_type ); //!< prepare an accepted socket

        public:
            socket_type        sock;  //!< internal system socket
            const socket_id_t  uuid;  //!< socket unique identifier
            const size_t       hkey;  //!< pre computed hash key
            const name_type    name;  //!< base64 uuid


        private:
            Y_DISABLE_COPY_AND_ASSIGN(bsd_socket);
            void on_init();
            static void *addr2addr(void *) throw();
            //! debug
            static std::ostream& __show_opt(std::ostream  &os,
                                            const void    *optval,
                                            const unsigned optlen);
        };


    }
}



#endif

