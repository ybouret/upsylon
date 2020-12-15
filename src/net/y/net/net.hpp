//! \file
#ifndef Y_NETWORK_INCLUDED
#define Y_NETWORK_INCLUDED 1

#include "y/net/socket/addr-ex.hpp"
#include "y/string.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! network instance for system
    //
    //__________________________________________________________________________
    class network : public singleton<network>
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________


        static bool           verbose;                 //!< mostly to debug
        static const uint16_t reserved_port;           //!< port < reserved_port: for system
        static const uint16_t first_user_port;         //!< port >= first_user_port: for user
        static const uint16_t final_user_port = 65535; //!< for information
        static const uint16_t user_port_width;         //!< final_user_port-first_user_port+1;

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! get host name
        string get_host_name() const;

        //! create a socket
        net::socket_type open(const net::ip_protocol, const net::ip_version);

        //! resolve the address, preserving the port
        void resolve( net::socket_address &ip, const string &s ) const;

        //! resolve the address, preserving the port, wrapper
        void resolve( net::socket_address &ip, const char   *s ) const;


        static net::ip_version ip_version(const string &); //!< "v4|v6" -> [v4|v6]
        static net::ip_version ip_version(const char   *); //!< "v4|v6" -> [v4|v6]

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(network);
        explicit network();
        virtual ~network() throw();
        
    public:
        Y_SINGLETON_DECL_WITH(object::life_time-13,network); //!< setup
        
    };

    //! macro to be used with network verbosity
#define Y_NET_VERBOSE(MSG) do {  \
if(network::verbose) {           \
Y_LOCK(network::access);         \
std::cerr << MSG << std::endl;   \
} } while(false);                

}

#endif

