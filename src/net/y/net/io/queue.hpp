//! \file
#ifndef Y_NET_IO_QUEUE_INCLUDED
#define Y_NET_IO_QUEUE_INCLUDED 1

#include "y/net/types.hpp"
#include "y/net/io/bytes.hpp"
#include "y/memory/buffers.hpp"

namespace upsylon
{
    namespace net
    {

        class io_queue : public net_object, public io_bytes
        {
        public:
            typedef memory::cblock data_block;

            explicit io_queue(const size_t bs);
            virtual ~io_queue() throw();
            
            void load(size_t bs);

            data_block data;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(io_queue);
        };


    }

}

#endif

