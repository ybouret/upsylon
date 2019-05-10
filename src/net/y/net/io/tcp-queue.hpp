//! \file
#ifndef Y_NET_IO_TCP_QUEUE_INCLUDED
#define Y_NET_IO_TCP_QUEUE_INCLUDED 1

#include "y/net/tcp/client.hpp"
#include "y/ptr/arc.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon
{
    namespace net
    {

        //! dynamic queue for tcp I/O
        class tcp_queue : public net_object
        {
        public:
            virtual ~tcp_queue() throw();                   //!< cleanup everything
            void           prune() throw();                 //!< clear pool
            virtual size_t size()  const throw() = 0;       //!< bytes for I/O
            virtual void   reset() throw() = 0;             //!< free content, keep resources
            virtual size_t comm(const tcp_client &) = 0;  //!< send or receive

        protected:
            explicit tcp_queue(const size_t bs); //!< setup with internal block size
            
            //------------------------------------------------------------------
            core::list_of<byte_node> bytes;      //!< list of bytes
        public:                                  //|
            const size_t             block_size; //!< internal block_size
        private:                                 //|
            size_t                   allocated;  //!< allocated>=block_size
        protected:                               //|
            uint8_t                 *buffer;     //!< linear buffer[block_size]
            core::pool_of<byte_node> bpool;      //!< pool of byte nodes
            byte_node * to_node(const uint8_t);  //!< dynamic memory handling
            void        clear() throw();         //!< put bytes into pool
            //------------------------------------------------------------------

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_queue);
        };



    }
}

#endif

