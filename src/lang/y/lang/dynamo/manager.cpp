
#include "y/lang/dynamo/manager.hpp"


namespace upsylon
{
    namespace Lang
    {
        DynamoManager:: ~DynamoManager() throw()
        {
        }

        DynamoManager:: DynamoManager() throw() :
        __loader(0),
        __compiler(0),
        __generator(0)
        {
        }

        

        DynamoLoader & DynamoManager:: loader()
        {
            if(__loader.is_empty())
            {
                __loader = new DynamoLoader();
            }
            return * __loader;
        }
        
        DynamoCompiler & DynamoManager:: compiler()
        {
            if( __compiler.is_empty() )
            {
                __compiler = new DynamoCompiler();
            }
            return * __compiler;
        }
        
        DynamoGenerator & DynamoManager:: generator()
        {
            if( __generator.is_empty() )
            {
                __generator = new DynamoGenerator();
            }
            return * __generator;
        }
        
        Syntax::Parser * DynamoManager:: link( Module *dynamoModule, DynamoSymbols *symbols)
        {
            assert( dynamoModule );
            Source               source(dynamoModule);
            auto_ptr<DynamoNode> dnode = DynamoNode::Load(source);
            return generator().build(*dnode,symbols);
        }

        Syntax::Parser * DynamoManager:: loadAndLink( Module *grammarModule, DynamoSymbols *symbols )
        {
            assert(grammarModule);
            auto_ptr<Module>      guard( grammarModule );
            auto_ptr<XNode>       xnode = loader().load(guard.yield());
            auto_ptr<DynamoNode>  dnode = compiler().compile(*xnode);
            xnode = 0;
            return generator().build(*dnode,symbols);
        }

    }

}


