//! \file
#ifndef Y_NET_IO_SEND_QUEUE_INCLUDED
#define Y_NET_IO_SEND_QUEUE_INCLUDED 1

#include "y/net/io/queue.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! pack user bytes into block
        //
        //______________________________________________________________________
        class send_queue : public io_queue
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
            size_t   packed()     const throw(); //!< packed data (written)
            void     defrag()           throw(); //!< try to defrag data
            void     sent(const size_t) throw(); //!< adjust after a successful send
            void     pack()             throw(); //!< transert bytes to buffer

            std::ostream        & display(std::ostream &) const;
            friend std::ostream & operator<<(std::ostream&,const send_queue&);

        protected:
            uint8_t       *origin;    //!< data
            size_t         offset;    //!< initial offset
            const uint8_t *current;   //!< first byte to send
            size_t         written;   //!< bytes to send
            uint8_t       *beginning; //!< first writable bytes
            size_t         available; //!< available bytes


        private:
            Y_DISABLE_COPY_AND_ASSIGN(send_queue);
            void transfer(size_t n) throw();
            void resetData() throw();
        };

    }

}

#endif
