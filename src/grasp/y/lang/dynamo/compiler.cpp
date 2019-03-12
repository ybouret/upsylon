#include "y/lang/dynamo/compiler.hpp"
#include "y/exception.hpp"


namespace upsylon
{
    namespace Lang
    {

        DynamoCompiler:: DynamoCompiler() : DynamoLoader(), Syntax::Analyzer()
        {
        }

        DynamoCompiler::~DynamoCompiler() throw()
        {
        }
        
    }

}

#if 0
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

        const char * DynamoCompiler:: top_kw[] =
        {
            "dynamo", "rule"
        };

        DynamoCompiler:: DynamoCompiler() :
        DynamoLoader(),
        parser(0),
        rules(),
        terms(),
        modules(),
        top_h(YOCTO_MPERF_FOR(top_kw))
        {
        }
        
        DynamoCompiler:: ~DynamoCompiler() throw()
        {
        }

        void DynamoCompiler:: reset() throw()
        {
            rules.free();
            terms.free();
            modules.free();
            parser = 0;
        }

        Syntax::Parser * DynamoCompiler:: compile( XNode &top )
        {
            reset();
            try
            {
                decl(top);
            }
            catch(...)
            {
                reset();
                throw;
            }
            return 0;
        }
        
    }
}
#endif

