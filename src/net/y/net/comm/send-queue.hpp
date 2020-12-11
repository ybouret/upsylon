//! \file
#ifndef Y_NET_COMM_SEND_QUEUE_INCLUDED
#define Y_NET_COMM_SEND_QUEUE_INCLUDED 1

#include "y/net/comm/queue.hpp"
#include "y/ios/ostream.hpp"

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
        //! pack user bytes into block, low-level but usable as ostream
        //
        //______________________________________________________________________
        class send_queue : public comm_queue, public ios::ostream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit send_queue(comm_block *block); //!< setup data/queue
            virtual ~send_queue() throw();          //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool         uploaded(tcp_client &);       //!< check if all could be sent

            //__________________________________________________________________
            //
            // ios::ostream interface
            //__________________________________________________________________
            virtual void write(char C);                      //!< optimized write
            virtual void flush() throw();                    //!< do nothing
            virtual void output(const void *, const size_t); //!< optimized write
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            std::ostream        & display(std::ostream &) const;               //!< display content
            friend std::ostream & operator<<(std::ostream&,const send_queue&); //!< display content

            const size_t writable;  //!< writable bytes
            const size_t readable;  //!< readable bytes
            
        private:
            size_t         invalid;   //!< invalid bytes
            uint8_t       *rw;        //!< first writable byte : ro+readable
            const uint8_t *ro;        //!< first read onnly    : **base+invalid

            Y_DISABLE_COPY_AND_ASSIGN(send_queue);
            void         reset_metrics() throw();
            virtual void reset_() throw(); //!< reset_metrics()
            void         write1(const uint8_t code) throw();
            void         writeN(const void *,const size_t) throw();
            void         defrag() throw();
        };

    }

}

#endif
