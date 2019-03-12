#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        
        DynamoCompiler:: DynamoCompiler() :
        DynamoLoader()
        {
        }
        
        DynamoCompiler:: ~DynamoCompiler() throw()
        {
        }


        Syntax::Parser * DynamoCompiler:: compile( XNode &top )
        {

            return 0;
        }
        
    }
}
