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
            size_t       uploaded(tcp_client &);                              //!< what was uploaded in one call
            size_t       uploaded(void *buffer, const size_t buflen) throw(); //!< number of copied bytes in one call
            
            //__________________________________________________________________
            //
            // ios::ostream interface
            //__________________________________________________________________
            virtual void write(char C);                      //!< optimized write
            virtual void flush() throw();                    //!< defragment
            virtual void output(const void *, const size_t); //!< optimized write of a block
            void         remove(const size_t n) throw();     //!< remove n bytes from readable, defrag and  load frome queue
            bool         sending()        const throw();     //!< size>0 || readable>0
            size_t       to_send()        const throw();     //!< size+readable
            
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
            void         write1(const uint8_t code)        throw(); //! one byte at rw, update status
            void         writeN(const void *,const size_t) throw(); //!< N bytes at rwm update status
            void         defrag()                          throw(); //!< defragment (invalid>0)
        };

    }

}

#endif
