
#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoCompiler:: DynamoCompiler() :
        DynamoLoader(),
        Syntax::Analyzer()
        {
        }
        
        DynamoCompiler:: ~DynamoCompiler() throw()
        {
        }
        
    }
}
