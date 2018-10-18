//! \file
#ifndef Y_IOS_STREAM_BUFFER_INCLUDED
#define Y_IOS_STREAM_BUFFER_INCLUDED 1

#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace ios
    {

        //! I/O stream buffer
        class stream_buffer : public memory::rw_buffer
        {
        public:
            virtual ~stream_buffer() throw();
            explicit stream_buffer(const size_t n);

            virtual const void *ro()     const throw();  //!< current position
            virtual size_t      length() const throw();  //!< available byte

            bool query( char & C) throw();

        protected:
            const size_t   maxi; //!< chunk size
            size_t         wlen; //!< wlen >= maxi
            uint8_t       *head; //!< first byte
            uint8_t       *curr; //!< current byte
            uint8_t       *last; //!< first invalid byte

        private:
            Y_DISABLE_COPY_AND_ASSIGN(stream_buffer);
        };
    }
}

#endif

