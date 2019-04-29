//! \file
#ifndef Y_NET_TCP_STREAM_INCLUDED
#define Y_NET_TCP_STREAM_INCLUDED 1

#include "y/net/tcp/cache.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"

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
            virtual ~tcp_istream() throw();          //!< destructor
            explicit tcp_istream(const tcp_link  &,  //
                                 const tcp_cache &); //!< setup

            virtual bool query( char &C ); //!< cache/read at most bufsize
            virtual void store( char C  ); //!< read back

        private:
            tcp_cache cache;

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
