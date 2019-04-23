//! \file
#ifndef Y_NETWORK_INCLUDED
#define Y_NETWORK_INCLUDED 1

#include "y/net/ip-addr.hpp"
#include "y/string.hpp"

namespace upsylon
{
    
    //! network instance for system
    class network : public singleton<network>
    {
    public:
        static bool verbose;          //!< mostly to debug
        string get_host_name() const; //!< get host name

        net::socket_type open(const net::ip_protocol proto, const net::ip_version version);

    private:
        Y_DISABLE_COPY_AND_ASSIGN(network);
        explicit network();
        virtual ~network() throw();
        friend class singleton<network>;

    public:
        static const at_exit::longevity life_time = object::life_time - 13; //!< life time

    };

}

#endif

