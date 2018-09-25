#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoCompiler:: ~DynamoCompiler() throw()
        {
        }

        DynamoCompiler:: DynamoCompiler() :
        parser( new DynamoParser() )
        {
            
        }

    }
}
