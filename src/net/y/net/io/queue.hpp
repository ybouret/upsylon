//! \file
#ifndef Y_NET_IO_QUEUE_INCLUDED
#define Y_NET_IO_QUEUE_INCLUDED 1

#include "y/net/types.hpp"
#include "y/net/io/bytes.hpp"
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
        class io_queue : public net_object, public io_bytes
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
            explicit io_queue(const size_t bs); //!< setup with minimal block size, reserve some bytes
            virtual ~io_queue() throw();        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void load(size_t bs); //!< data --> bytes


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            data_block data; //!< data
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(io_queue);
        };


    }

}

#endif

