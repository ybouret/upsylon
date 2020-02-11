//! \file
#ifndef Y_INFORMATION_ENTROPY_INCLUDED
#define Y_INFORMATION_ENTROPY_INCLUDED 1

#include "y/memory/buffer.hpp"

namespace upsylon {

    namespace information {

        class entropy
        {
        public:
            explicit entropy() throw();
            virtual ~entropy() throw();

            void reset() throw();
            
            size_t       & operator[]( const uint8_t u ) throw();
            const size_t & operator[]( const uint8_t u ) const throw();

            entropy & update( const void *data, const size_t size ) throw();
            entropy & operator<<( const char *text ) throw();
            entropy & operator<<( const memory::ro_buffer &buff ) throw();

            double operator*() const throw();

            double of( const void *data, const size_t size ) throw();
            double of( const char *text ) throw();
            double of( const memory::ro_buffer &buff) throw();

        private:
            size_t frequency[256];
            Y_DISABLE_COPY_AND_ASSIGN(entropy);

        };

    }
}

#endif

