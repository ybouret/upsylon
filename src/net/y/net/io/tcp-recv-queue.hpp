//! \file
#ifndef Y_NET_IO_TCP_RECV_QUEUE_INCLUDED
#define Y_NET_IO_TCP_RECV_QUEUE_INCLUDED 1

#include "y/net/io/tcp-queue.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace net
    {

        //! tcp queue to recieve data
        class tcp_recv_queue : public tcp_queue, public inode<tcp_recv_queue>
        {
        public:
            virtual ~tcp_recv_queue() throw();        //!< cleanup
            explicit tcp_recv_queue(const size_t bs); //!< setup
            
            virtual void    reset() throw();                  //!< clear
            virtual size_t  size() const throw();             //!< available bytes
            virtual size_t  comm( const tcp_client &client ); //!< recv block and append to bytes

            size_t  pop(void *ptr,size_t num) throw();        //!< pop at most num first bytes
            size_t  peek(void *ptr,size_t num) const throw(); //!< peek at most num first bytes
            void    skip(size_t num) throw();                 //!< skip num bytes, nun<=size()
            char getch() throw();   //!< return first char in cache
            void putch(char C);     //!< unread char

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_recv_queue);
        };
    }
}

#endif

