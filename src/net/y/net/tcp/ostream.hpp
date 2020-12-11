

//! \file
#ifndef Y_NET_TCP_OSTREAM_INCLUDED
#define Y_NET_TCP_OSTREAM_INCLUDED 1

#include "y/net/tcp/stream.hpp"
#include "y/net/comm/send-queue.hpp"
#include "y/ios/ostream.hpp"


namespace upsylon
{
    namespace net
    {

        //! ostream decorator for tcp_client
        class tcp_ostream : public ios::ostream, public tcp_stream
        {
        public:
            virtual ~tcp_ostream() throw();         //!< destructor
            explicit tcp_ostream(const tcp_link &,  //|
                                 comm_block      *); //!< setup

            virtual void write( char C ); //!< direct write
            virtual void flush();         //!< flush
            virtual void output(const void *, const size_t); //!< output

            send_queue Q; //!< sending queue
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_ostream);
        };
        
    }

}

#endif

