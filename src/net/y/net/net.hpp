//! \file
#ifndef Y_NETWORK_INCLUDED
#define Y_NETWORK_INCLUDED 1

#include "y/net/socket/addr-ex.hpp"
#include "y/string.hpp"

namespace upsylon
{
    
    //! network instance for system
    class network : public singleton<network>
    {
    public:
        static bool verbose;          //!< mostly to debug
        string get_host_name() const; //!< get host name

        //! create a socket
        net::socket_type open(const net::ip_protocol proto, const net::ip_version version);
        

        //! resolve the address, preserving the port
        void resolve( net::socket_address &ip, const string &s ) const;

        //! resolve the address, preserving the port, wrapper
        void resolve( net::socket_address &ip, const char   *s ) const;

        static const uint16_t reserved_port;           //!< port < reserved_port: for system
        static const uint16_t first_user_port;         //!< port >= first_user_port: for user
        static const uint16_t final_user_port = 65535; //!< for information
        static const uint16_t user_port_width;         //!< final_user_port-first_user_port+1;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(network);
        explicit network();
        virtual ~network() throw();
        friend class singleton<network>;

    public:
        static const at_exit::longevity life_time = object::life_time - 13; //!< life time
    };

    //! macro to be used with network verbosity
#define Y_NET_VERBOSE(CODE) do {  \
if(network::verbose) {            \
Y_LOCK(network::access); CODE;    \
} } while(false);                

}

#endif

