
//! \file
#ifndef Y_NET_TCP_ISTREAM_INCLUDED
#define Y_NET_TCP_ISTREAM_INCLUDED 1

#include "y/net/tcp/stream.hpp"
#include "y/net/comm/recv-queue.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    namespace net
    {
        //______________________________________________________________________
        //
        //
        //! istream decorator for simple tcp client
        //
        //______________________________________________________________________
        class tcp_istream : public ios::istream, public tcp_stream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit tcp_istream(const tcp_link &,
                                 comm_block     *); //!< setup
            virtual ~tcp_istream() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual bool query(char &C); //!< use Q or download
            virtual void store(char  C); //!< push front

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            recv_queue Q; //!< receiving queue

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_istream);
        };
        

    }

}

#endif
