#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoInfo:: ~DynamoInfo() throw()
        {

        }


        DynamoInfo:: DynamoInfo( const Tag &id ) throw() : moduleID(id) {}

        DynamoInfo:: DynamoInfo( const DynamoInfo &other) throw() :
        moduleID(other.moduleID)
        {
        }

        
    }


}
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
            rules.free();
            terms.free();
            modules.free();
            
            return 0;
        }
        
    }
}
