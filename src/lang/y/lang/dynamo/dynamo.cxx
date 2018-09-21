#include "y/program.hpp"
#include "y/lang/compiler.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"


using namespace upsylon;
using namespace Lang;


class DynamoParser : public Syntax::Parser
{
public:

    inline explicit DynamoParser() : Syntax::Parser( "Dynamo" )
    {




        root.endl("endl","[:endl:]" );
        root.drop("ws",  "[:blank:]");
    }



private:
    Y_DISABLE_COPY_AND_ASSIGN(DynamoParser);
};

class DynamoCompiler : public Lang::Compiler
{
public:

    inline explicit DynamoCompiler() : Lang::Compiler( new DynamoParser() )
    {
    }

    inline virtual ~DynamoCompiler() throw()
    {
    }

};


Y_PROGRAM_START()
{
    DynamoCompiler dynamo;
    
}
Y_PROGRAM_END()

