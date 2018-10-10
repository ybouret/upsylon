//! \file
#ifndef Y_HASHING_RMD_INCLUDED
#define Y_HASHING_RMD_INCLUDED 1


#include "y/os/platform.hpp"

namespace upsylon
{
    
    namespace hashing
    {
     
        //! input class for rmd
        class rmd
        {
        public:
            rmd() throw();  //!< setup
            ~rmd() throw(); //!< destructor
            
          
            //! reset all fields
            void reset() throw();
            
            //! store in bytes, then in words
            /**
             return true when 16 * 4 bytes are ready
             */
            bool store( uint8_t b ) throw();
            
            //! return a full block
            const uint32_t *block() const throw();
            
            //! assemble the last block
            const uint8_t *flush() throw();

            //! internal length
            const size_t   length;
            
            uint32_t lswlen() const throw(); //!< least significant word len
            uint32_t mswlen() const throw(); //!<  most signigicant word len
            
        private:
            size_t   nx;
            size_t   nb;
            uint32_t X[16];  //!< words store
            uint8_t  B[4];   //!< bytes store
            Y_DISABLE_COPY_AND_ASSIGN(rmd);
            
        };
        
    }
    
}

#endif
