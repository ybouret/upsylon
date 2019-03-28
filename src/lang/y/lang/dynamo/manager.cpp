
#include "y/lang/dynamo/manager.hpp"


namespace upsylon
{
    namespace Lang
    {
        DynamoManager:: ~DynamoManager() throw()
        {
        }

        DynamoManager:: DynamoManager() throw() :
        ld(0)
        {
        }

        

        DynamoLoader & DynamoManager:: loader()
        {
            if(ld.is_empty())
            {
                ld = new DynamoLoader();
            }
            return *ld;
        }
        
        Syntax::Parser * DynamoManager:: link( Module *dynamoModule, DynamoSymbols *symbols)
        {
            assert( dynamoModule );
            Source               source(dynamoModule);
            auto_ptr<DynamoNode> dnode = DynamoNode::Load(source);
            return build(*dnode,symbols);
        }

        Syntax::Parser * DynamoManager:: loadAndLink( Module *grammarModule, DynamoSymbols *symbols )
        {
            assert(grammarModule);
            DynamoCompiler cmp;
            auto_ptr<Module>      guard( grammarModule );
            DynamoLoader         &dynld = loader();
            auto_ptr<XNode>       xnode = dynld.load(guard.yield());
            auto_ptr<DynamoNode>  dnode = cmp.compile(*xnode);
            return build(*dnode,symbols);
        }

    }

}


