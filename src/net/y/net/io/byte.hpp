//! \file
#ifndef Y_NET_IO_BYTE_INCLUDED
#define Y_NET_IO_BYTE_INCLUDED 1


#include "y/memory/magazine.hpp"

namespace upsylon
{
    namespace net
    {

        class io_byte
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef memory::magazine<io_byte> supply;     //!< alias
            typedef supply::list_             list_type;  //!< alias

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static supply & instance();
            static supply & location() throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint8_t code;
            io_byte      *next;
            io_byte      *prev;

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(io_byte);
            io_byte(const uint8_t) throw();
            ~io_byte() throw();
            friend class memory::magazine<io_byte>;
        };



    }

    Y_MAGAZINE_DECL(net::io_byte);

}


#endif
