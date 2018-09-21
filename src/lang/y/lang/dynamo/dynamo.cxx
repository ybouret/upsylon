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
        //----------------------------------------------------------------------
        //
        // top level rule
        //
        //----------------------------------------------------------------------

        AGG  &dynamo = agg("dynamo");
        RULE &RuleID = term("RuleID", "[:word:]+");

        hook<Lexical::jString>("string");
        RULE &STR   = term("string");

        hook<Lexical::rString>("rstring");
        RULE &RAW   = term("rstring");
        

        root.endl("endl","[:endl:]" );
        root.drop("ws",  "[:blank:]");

        checkValidity();
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

