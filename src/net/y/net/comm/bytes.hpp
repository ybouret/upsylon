//! \file
#ifndef Y_NET_COMM_BYTES_INCLUDED
#define Y_NET_COMM_BYTES_INCLUDED 1


#include "y/net/comm/byte.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace net
    {

        typedef comm_byte::list_type comm_bytes_type; //!< alias

        //______________________________________________________________________
        //
        //
        //! dynamic cached list of byte
        //
        //______________________________________________________________________
        class comm_bytes : public comm_bytes_type
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit comm_bytes() throw();                        //!< setup empty
            explicit comm_bytes(size_t n, const as_capacity_t &); //!< setup with capacity
            virtual ~comm_bytes() throw();                        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void         reserve(size_t n);                             //!< populate pool with extra bytes
            void         push(const uint8_t code);                      //!< push back one byte
            void         push(const void *buffer, const size_t buflen); //!< push back a buffer
            void         push(const char   *text);                      //!< push back some text
            void         push(const memory::ro_buffer &);               //!< push back a r/o buffer
            void         clear() throw();                               //!< free content, keep memory
            uint8_t      pop()   throw();                               //!< return content of removed head
            comm_byte   *rig(const uint8_t);                            //!< from pool or acquire

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            comm_bytes_type pool; //!< local pool

        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_bytes);
        };

    }

}

#endif

