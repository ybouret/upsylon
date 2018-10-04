
#include "y/lang/dynamo.hpp"
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {

        static inline Syntax::Parser * __From( Syntax::Node &dynamo, const bool verbose )
        {
            DynamoGenerator gen;
            return gen.create(dynamo,verbose);
        }

        Syntax::Parser *Dynamo:: FromFile( const string &filename, const bool verbose)
        {
            auto_ptr<Syntax::Node> dynamo = 0;
            {
                DynamoCompiler  dynCompiler;
                dynamo = dynCompiler.process( Module::OpenFile(filename) );
            }
            return __From(*dynamo,verbose);
        }

        static inline Syntax::Parser *__From( Module *module, const bool verbose )
        {
            assert(module);
            DynamoParser           dynParser;
            Source                 source(module);
            auto_ptr<Syntax::Node> dynamo = Syntax::Node::Load(source,dynParser);
            DynamoGenerator gen;
            return __From(*dynamo,verbose);
        }

        Syntax::Parser * Dynamo:: FromData( const string &filename, const bool verbose)
        {
            return __From( Module::OpenFile(filename), verbose);
        }

        Dynamo:: ~Dynamo() throw()
        {
        }


    }
}
