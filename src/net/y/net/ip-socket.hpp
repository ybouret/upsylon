//! \file
#ifndef Y_NET_IP_SOCKET_INCLUDED
#define Y_NET_IP_SOCKET_INCLUDED 1

#include "y/net/net.hpp"

namespace upsylon
{
    namespace net
    {
        enum shutdown_type
        {
            sd_send,
            sd_recv,
            sd_both
        };

#if defined(Y_BSD)
        typedef int socket_boolean;
#endif

#if defined(Y_WIN)
        typedef BOOL socket_boolean;
#endif
        extern const socket_boolean socket_true;
        extern const socket_boolean socket_false;

        //! low level socket API
        class ip_socket
        {
        public:
            virtual ~ip_socket() throw();
            explicit ip_socket(const ip_protocol protocol,
                               const ip_version  version);

            void async(); //!< set non blocking mode
            void shutdown( const shutdown_type how ) throw(); //!< shutdown

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
            socket_type sock;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ip_socket);
        };
    }
}



#endif

