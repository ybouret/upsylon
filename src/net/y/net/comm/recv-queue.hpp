
//! \file
#ifndef Y_NET_COMM_RECV_QUEUE_INCLUDED
#define Y_NET_COMM_RECV_QUEUE_INCLUDED 1

#include "y/net/comm/queue.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //! forward declaration
        //______________________________________________________________________
        class tcp_client;

        //______________________________________________________________________
        //
        //
        //!   block input --> queue of byte, low-level but usable as istream
        //
        //______________________________________________________________________
        class recv_queue : public comm_queue, public ios::istream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit recv_queue(comm_block *); //!< setup data/queue
            virtual ~recv_queue() throw();     //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t download(tcp_client &);        //!< receive from tcp_client

            //__________________________________________________________________
            //
            // ios::istream interface
            //__________________________________________________________________
            virtual bool is_active()   throw();
            virtual bool query(char &);
            virtual void store(char C);
            
            //__________________________________________________________________
            //
            // debug methods
            //__________________________________________________________________
            size_t justLoad(const void *, const size_t); //!< copy and bring, to debug
            void   demoLoad(const void *, const size_t); //!< copy all and bring all
            void   demoLoad(const char *);               //!< demo load text
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(recv_queue);
            virtual void reset_() throw();  
            size_t       bring_(size_t bs); //!< data --> queue
        };

    }

}

#endif
