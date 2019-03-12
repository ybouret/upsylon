//! \file
#ifndef Y_DYNAMO_GENERATOR_INCLUDED
#define Y_DYNAMO_GENERATOR_INCLUDED 1

#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {

        class DynamoGenerator
        {
        public:
            virtual ~DynamoGenerator() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoGenerator);
        };
    }

}

#endif

