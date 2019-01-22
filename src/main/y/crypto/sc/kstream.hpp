//! \file
#ifndef Y_CRYPTO_KSTREAM_INCLUDED
#define Y_CRYPTO_KSTREAM_INCLUDED 1

#include "y/object.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace crypto
    {
        //! stream of keyed bytes
        class kstream
        {
        public:
            virtual ~kstream() throw(); //!< destructor

            //! schedule internal contexts
            virtual void    schedule( const memory::ro_buffer &key ) throw() = 0;

            //! call a byte to mask
            virtual uint8_t call() throw() = 0;

            //! encode a byte
            uint8_t operator()(const uint8_t B) throw();

            //! encode a memory region
            void operator()( void *target, const void *source, const size_t length) throw();


        protected:
            explicit kstream() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(kstream);
        };
    }
}

#endif

