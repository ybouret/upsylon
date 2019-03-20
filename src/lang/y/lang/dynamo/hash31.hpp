//! \file
#ifndef Y_DYNAMO_HASH31_INCLUDED
#define Y_DYNAMO_HASH31_INCLUDED 1

#include "y/hashing/sha1.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! 31-bits hasher
        class DynamoHash31
        {
        public:
            typedef hashing::sha1 Hasher; //!< alias

            explicit DynamoHash31() throw(); //!< ctor
            virtual ~DynamoHash31() throw(); //!< dtor

            Hasher h; //!< internal hasher

            //! hash and keep 31 bits
            int32_t operator()( const memory::ro_buffer & ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoHash31);
        };
    }
}

#endif

