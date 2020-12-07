//! \file
#ifndef Y_NET_IO_BYTES_INCLUDED
#define Y_NET_IO_BYTES_INCLUDED 1


#include "y/net/io/byte.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace net
    {

        typedef io_byte::list_type io_bytes_type; //!< alias

        //______________________________________________________________________
        //
        //
        //! dynamic cached list of byte
        //
        //______________________________________________________________________
        class io_bytes : public io_bytes_type
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit io_bytes() throw();                        //!< setup empty
            explicit io_bytes(size_t n, const as_capacity_t &); //!< setup with capacity
            virtual ~io_bytes() throw();                        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void     reserve(size_t n); //!< populate pool with extra bytes


            io_bytes & operator<<(const uint8_t code);                 //!< push back one byte
            void       push(const void *buffer, const size_t buflen);  //!< push back a buffer
            io_bytes & operator<<(const char   *text);                 //!< push back some text
            io_bytes & operator<<(const memory::ro_buffer &);          //!< push back a r/o buffer
            void       clear() throw();                                 //!< free content, keep memory
            uint8_t    pop()   throw();                                 //!< return content of removed head

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            io_bytes_type pool; //!< local pool

        private:
            Y_DISABLE_COPY_AND_ASSIGN(io_bytes);
        };

    }

}

#endif

