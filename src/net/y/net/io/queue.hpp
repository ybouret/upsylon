#ifndef Y_NET_IO_QUEUE_INCLUDED
#define Y_NET_IO_QUEUE_INCLUDED 1

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
            explicit tcp_queue( const size_t bs ); //!< setup with internal block size
            virtual ~tcp_queue() throw();          //!< cleanup everything

            void free() throw(); //!< put bytes into pool
            void trim() throw(); //!< clear pool


        protected:
            core::list_of<byte_node> bytes;      //!< list of bytes
        public:                                  //|
            const size_t             block_size; //!< internal block_size
        private:                                 //|
            size_t                   allocated;  //!< allocated>=block_size
        protected:                               //|
            uint8_t                 *buffer;     //!< linear buffer[block_size]
            core::pool_of<byte_node> bpool;      //!< pool of byte nodes

            byte_node * to_node(const uint8_t code);

            Y_DISABLE_COPY_AND_ASSIGN(tcp_queue);
        };

        //! tcp queue to recieve data
        class tcp_recv_queue : public tcp_queue
        {
        public:
            virtual ~tcp_recv_queue() throw();        //!< cleanup
            explicit tcp_recv_queue(const size_t bs); //!< setup

            size_t  load( const tcp_client &client );        //!< recv block and get data

            size_t  size() const throw();                     //!< available bytes
            size_t  pop(void *ptr,size_t num) throw();        //!< pop at most num first bytes
            size_t  peek(void *ptr,size_t num) const throw(); //!< peek at most num first bytes
            
            char getch() throw();   //!< return first char in cache
            void putch(char C);     //!< unread char
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_recv_queue);
        };


    }
}

#endif

