//! \file

#ifndef Y_NET_COMM_CACHE_INCLUDED
#define Y_NET_COMM_CACHE_INCLUDED 1

#include "y/net/net.hpp"
#include "y/memory/dblock.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! internal cache of comm blocks
        //
        //______________________________________________________________________
        class comm_cache :
        public singleton<comm_cache>,
        public memory::dblocks
        {
        public:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_cache);
            explicit comm_cache();
            virtual ~comm_cache() throw();
            
        public:
            Y_SINGLETON_DECL_WITH(network::life_time-1,comm_cache); //!< setup

        };

        

    }
}


#endif

