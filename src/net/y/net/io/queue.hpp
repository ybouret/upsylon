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

        class queue : public io_bytes
        {
        public:
            typedef memory::cblock block_type;

            explicit queue(size_t n);
            virtual ~queue() throw();

            block_type comm;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(queue);
        };

    }

}

#endif

