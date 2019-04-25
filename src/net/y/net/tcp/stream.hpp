//! \file
#ifndef Y_NET_TCP_STREAM_INCLUDED
#define Y_NET_TCP_STREAM_INCLUDED 1

#include "y/net/tcp/client.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon
{
    namespace net
    {

        //! istream decorator for tcp_client
        class tcp_istream : public ios::istream
        {
        public:
            static const size_t bufsiz = 256; //!< internal size
            typedef core::node_of<uint8_t>       byte_node; //!< preloaded node
            typedef core::list_of_cpp<byte_node> byte_list; //!< list of chars
            typedef core::pool_of_cpp<byte_node> byte_pool; //!< pool of chars

            virtual ~tcp_istream() throw();              //!< destructor
            explicit tcp_istream( const tcp_link &conn); //!< setup memory

            virtual bool query( char &C ); //!< cache/read at most bufsize
            virtual void store( char C  ); //!< read back

        private:
            const tcp_link link;
            byte_list      cache;
            byte_pool      zbulk;

            bool           load();
            Y_DISABLE_COPY_AND_ASSIGN(tcp_istream);
        };
    }
}

#endif
