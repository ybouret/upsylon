//! \file
#ifndef Y_NET_TCP_CACHE_INCLUDED
#define Y_NET_TCP_CACHE_INCLUDED 1


#include "y/net/tcp/client.hpp"
#include "y/memory/slab.hpp"
#include "y/core/list.hpp"

namespace upsylon
{
    namespace net
    {
        //! reusable cache for tcp_istream
        class tcp_cache_ : public net_object
        {
        public:
            
            typedef memory::slab_of<byte_node> byte_slab; //!< alias
            typedef core::list_of<byte_node>   byte_list; //!< alias

            explicit tcp_cache_(const size_t n);    //!< setup memory
            virtual ~tcp_cache_() throw();          //!< reset and release
            void     reset() throw();               //!< empty cache into pool
            bool     load( const tcp_client &cln ); //!< load an empty cache with something
            size_t   size() const throw();          //!< content.size

            char getch() throw();   //!< return first char in cache
            void putch(char C);     //!< try to unread

        private:                    //
            byte_list    content;   //!< list of read bytes
        public:                     //
            const size_t capacity;  //!< total capacity
        private:                    //
            const size_t s_offset;  //
            const size_t s_length;  //
        public:                     //
            const size_t allocated; //!< allocated global memory
        private:                    //
            void        *buffer;    //
            byte_slab    pool;      //

            Y_DISABLE_COPY_AND_ASSIGN(tcp_cache_);
        };

        //! internal creation
        tcp_cache_                 *tcp_cache_new( const size_t n );
       // typedef arc_ptr<tcp_cache_> tcp_cache; //!< alias

    }

}

#endif


