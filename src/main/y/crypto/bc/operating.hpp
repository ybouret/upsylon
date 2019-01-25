//! \file
#ifndef Y_CRYPTO_BC_OPERATING_INCLUDED
#define Y_CRYPTO_BC_OPERATING_INCLUDED 1

#include "y/crypto/bc/ciphers.hpp"

namespace upsylon
{
    namespace crypto
    {
        //! use block ciphers to manage operating modes
        class operating
        {
        public:
            virtual ~operating() throw(); //!< destructor

            //! initialize with IV
            virtual void initialize( ciphers &c, const digest &IV ) throw() = 0;
            
            //! the operating mode for a cipher
            virtual void write_block( ciphers &c, void *output, const void *input ) throw() = 0;

            //! use ciphers flush
            void         flush_block( ciphers &c, void *output, const void *input, const size_t length ) throw();

            //! encode a whole chunk, for any length
            void         write( ciphers &c, void *output, const void *input, size_t length ) throw();

        protected:
            explicit operating() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(operating);
        };

    }
    
}

#endif

