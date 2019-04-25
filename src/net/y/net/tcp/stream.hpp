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
        //! base class
        class tcp_stream
        {
        public:
            virtual ~tcp_stream() throw(); //!< destructor

        protected:
            const tcp_link link;                          //!< shared pointer
            explicit tcp_stream(const tcp_link&) throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_stream);
        };


        //! istream decorator for tcp_client TODO: create dedicated cache
        class tcp_istream : public ios::istream, public tcp_stream
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
            byte_list      cache;
            byte_pool      zbulk;

            bool           load();
            Y_DISABLE_COPY_AND_ASSIGN(tcp_istream);
        };

        //! ostream decorator for tcp_client
        class tcp_ostream : public ios::ostream, public tcp_stream
        {
        public:
            virtual ~tcp_ostream() throw();                     //!< destructor
            explicit tcp_ostream(const tcp_link &conn) throw(); //!< setup

            virtual void write( char C ); //!< direct write
            virtual void flush();         //!< do nothing

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tcp_ostream);
        };
    }
}

#endif
