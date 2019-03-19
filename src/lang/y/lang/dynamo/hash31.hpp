//! \file

#ifndef Y_DYNAMO_HASH31_INCLUDED
#define Y_DYNAMO_HASH32_INCLUDED 1

#include "y/hashing/sha1.hpp"

namespace upsylon
{
    namespace Lang
    {
        class DynamoHash31
        {
        public:
            typedef hashing::sha1 Hasher;

            explicit DynamoHash31() throw();
            virtual ~DynamoHash31() throw();

            Hasher h;

            int32_t operator()( const memory::ro_buffer & ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoHash31);
        };
    }
}

#endif

