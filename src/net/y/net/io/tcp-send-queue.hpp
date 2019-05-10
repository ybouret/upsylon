
//! \file
#ifndef Y_NET_IO_TCP_SEND_QUEUE_INCLUDED
#define Y_NET_IO_TCP_SEND_QUEUE_INCLUDED 1

#include "y/net/io/tcp-queue.hpp"
#include "y/core/node.hpp"

namespace upsylon
{
    namespace net
    {

        //! tcp queue to send data
        class tcp_send_queue : public tcp_queue, public core::inode<tcp_send_queue>
        {
        public:
            virtual ~tcp_send_queue() throw();                 //!< cleanup
            explicit tcp_send_queue(const size_t bs);          //!< setup

            virtual void   reset()  throw();                   //!< re-link and clear()
            virtual size_t size() const throw();               //!< to_send + bytes.size()
            virtual size_t comm(const tcp_client &);           //!< send data and update

            size_t         buffer_space() const throw();        //!< block_size - to_send
            void           push(const void *ptr, size_t num);   //!< append data in buffer and bytes
            void           push(const char C );                 //!< append one char
            void           push(const char *text);              //!< append text
            void           push(const memory::ro_buffer &buff); //!< append buffer

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_send_queue);
            uint8_t *current;   //!< current addresse to be sent
            size_t   to_send;   //!< number of bytes to send
            uint8_t *available; //!< available address
            size_t   remaining; //!< remaining bytes in buffer

            void defrag() throw(); //!< optimize space

        };
    }
}

#endif

