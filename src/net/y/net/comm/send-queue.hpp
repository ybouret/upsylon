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
            explicit send_queue(const size_t bs); //!< setup data/queue
            virtual ~send_queue() throw();        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            size_t       packed()       const throw(); //!< packed data (written)
            void         defrag()             throw(); //!< try to defrag data
            void         update(const size_t) throw(); //!< update after a successful send
            void         pack()               throw(); //!< transfer bytes to data
            bool         uploaded(tcp_client &);       //!< check if all could be sent

            //__________________________________________________________________
            //
            // ios::ostream interface
            //__________________________________________________________________
            virtual void write(char C);                      //!< push(C)
            virtual void flush() throw();                    //!< pack()
            virtual void output(const void *, const size_t); //!< push(...)
            
            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            std::ostream        & display(std::ostream &) const;               //!< display content
            friend std::ostream & operator<<(std::ostream&,const send_queue&); //!< display content



        private:
            uint8_t       *origin;    //!< data
            size_t         offset;    //!< initial offset
            const uint8_t *current;   //!< first byte to send
            size_t         written;   //!< bytes to send
            uint8_t       *beginning; //!< first writable bytes
            size_t         available; //!< available bytes


        private:
            Y_DISABLE_COPY_AND_ASSIGN(send_queue);
            void         transfer(size_t n) throw(); // queue -> data
            void         resetData() throw();        // clear data
            virtual void reset_() throw();           // resetData
        };

    }

}

#endif
