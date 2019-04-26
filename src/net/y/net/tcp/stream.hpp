//! \file
#ifndef Y_NET_TCP_STREAM_INCLUDED
#define Y_NET_TCP_STREAM_INCLUDED 1

#include "y/net/tcp/client.hpp"
#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/memory/slab.hpp"

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

        class tcp_cache : public net_object
        {
        public:
            struct byte_node
            {
                byte_node *prev;
                byte_node *next;
                uint8_t    code;
            };
            typedef memory::slab<byte_node>  byte_slab;
            typedef core::list_of<byte_node> byte_list;

            explicit tcp_cache(const size_t n);     //!< setup memory
            virtual ~tcp_cache() throw();           //!< reset and release
            void     reset() throw();               //!< empty cache into pool
            bool     load( const tcp_client &cln ); //!< load an empty cache with something
            size_t   size() const throw();          //!< content.size

            char getch() throw(); //!< return first char in cache
            void putch(char C);   //!< try to unread

        private:
            byte_list    content;  //!< list of read bytes
        public:
            const size_t capacity; //!< total capacity
        private:
            const size_t s_offset;
            const size_t s_length;
        public:
            const size_t       allocated;
        private:
            uint8_t     *buffer;
            byte_slab    pool;    //!< pool of bytes

            Y_DISABLE_COPY_AND_ASSIGN(tcp_cache);
        };

        typedef arc_ptr<tcp_cache> tcp_input_cache; //!< alias


        //! istream decorator for tcp_client TODO: create dedicated cache
        class tcp_istream : public ios::istream, public tcp_stream
        {
        public:
            virtual ~tcp_istream() throw();              //!< destructor
            explicit tcp_istream(const tcp_link        &,
                                 const tcp_input_cache &); //!< setup

            virtual bool query( char &C ); //!< cache/read at most bufsize
            virtual void store( char C  ); //!< read back

        private:
            tcp_input_cache cache;

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
