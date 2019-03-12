//! \file
#ifndef Y_DYNAMO_COMPILER_INCLUDED
#define Y_DYNAMO_COMPILER_INCLUDED 1

#include "y/lang/dynamo/loader.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! build an intermediate tree
        class DynamoCompiler : public DynamoLoader
        {
        public:
            explicit DynamoCompiler();
            virtual ~DynamoCompiler() throw();
            

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(DynamoCompiler);
            
        };
        
    }
}

#endif

