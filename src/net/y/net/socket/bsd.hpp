//! \file
#ifndef Y_NET_BSD_SOCKET_INCLUDED
#define Y_NET_BSD_SOCKET_INCLUDED 1

#include "y/net/net.hpp"

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
            virtual ~bsd_socket() throw();                  //!< destructor
            explicit bsd_socket(const ip_protocol protocol, //
                                const ip_version  version); //!< setup

            void async();                               //!< set non blocking mode
            void shutdown(const shutdown_type) throw(); //!< shutdown
            static const char *sd_text(const shutdown_type) throw(); //!< for shutdown info


            //! wrapper to setsocketopt
            void setopt(const int      level,
                        const int      optname,
                        const void    *optval,
                        const unsigned optlen);

            //! wrapper to boolean values
            template <typename T> inline void setopt( const int level, const int optname, const T value )
            {
                setopt(level,optname, &value, sizeof(T) );
            }

            void on(  const int level, const int optname ); //!< turn flag on
            void off( const int level, const int optname ); //!< turn flag off

        protected:
            socket_type sock; //!< internal system socket
            explicit bsd_socket( const socket_type accepted ); //!< prepare an accepted socket

        private:
            Y_DISABLE_COPY_AND_ASSIGN(bsd_socket);
            void on_init();
        };


    }
}



#endif

