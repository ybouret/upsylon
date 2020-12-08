//! \file
#ifndef Y_NET_COMM_QUEUE_INCLUDED
#define Y_NET_COMM_QUEUE_INCLUDED 1

#include "y/net/comm/bytes.hpp"
#include "y/memory/cblock.hpp"

namespace upsylon
{
    namespace net
    {

        //______________________________________________________________________
        //
        //
        //! queue to use buffer <-> bytes
        //
        //______________________________________________________________________
        class comm_queue : public comm_bytes
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef memory::cblock data_block;  //!< buffer of data

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit comm_queue(const size_t bs); //!< setup with minimal block size, reserve some bytes
            virtual ~comm_queue() throw();        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t         block_size() const  throw(); //!< available I/O block size
            void           reset()             throw(); //!< reseting queue+data

        protected:
            data_block   data; //!< data
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_queue);
            virtual void reset_() throw() = 0;
        };


    }

}

#endif

