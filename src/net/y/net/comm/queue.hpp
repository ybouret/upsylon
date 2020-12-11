//! \file
#ifndef Y_NET_COMM_QUEUE_INCLUDED
#define Y_NET_COMM_QUEUE_INCLUDED 1

#include "y/net/comm/bytes.hpp"
#include "y/memory/cblock.hpp"
#include "y/memory/dblock.hpp"

namespace upsylon
{
    namespace net
    {

        typedef memory::dblock comm_block; //!< alias

        //______________________________________________________________________
        //
        //
        //! low-level queue to use buffer <-> bytes
        /**
         a comm_queue is some comm_bytes with a data block
         */
        //
        //______________________________________________________________________
        class comm_queue : public comm_bytes
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit comm_queue(comm_block *); //!< setup with a dblock and adjust bytes
            virtual ~comm_queue() throw();     //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void reset() throw(); //!< clear queue, call reset_(), zero data
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            comm_block::pointer data; //!< query data from comm_cache
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_queue);
            virtual void reset_() throw() = 0; //!< reset internal derived data
        };


    }

}

#endif

