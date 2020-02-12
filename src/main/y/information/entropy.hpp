//! \file
#ifndef Y_INFORMATION_ENTROPY_INCLUDED
#define Y_INFORMATION_ENTROPY_INCLUDED 1

#include "y/memory/buffer.hpp"

namespace upsylon {

    namespace information {

        //! computing entropy
        class entropy
        {
        public:
            explicit entropy() throw(); //!< setup
            virtual ~entropy() throw(); //!< cleanup
            void     reset()   throw(); //!< clean state
            
            size_t       & operator[]( const uint8_t u ) throw();       //!< safe access
            const size_t & operator[]( const uint8_t u ) const throw(); //!< safe access

            entropy & update( const void *data, const size_t size ) throw(); //!< update with block of data
            entropy & operator<<( const char *text ) throw();                //!< update with text
            entropy & operator<<( const memory::ro_buffer &buff ) throw();   //!< update with memory buffer

            double operator*() const throw(); //!< compute value

            double of( const void *data, const size_t size ) throw(); //!< reset/compute
            double of( const char *text ) throw();                    //!< reset/compute
            double of( const memory::ro_buffer &buff) throw();        //!< reset/compute

        private:
            size_t frequency[256];
            Y_DISABLE_COPY_AND_ASSIGN(entropy);

        };

    }
}

#endif

