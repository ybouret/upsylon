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

        RULE &END      = mark(';');


        //----------------------------------------------------------------------
        //
        // moduleID
        //
        //----------------------------------------------------------------------
        dynamo << (acting("DeclModule") << term("Module","[.][:word:]+") << END);



        //----------------------------------------------------------------------
        //
        // prepare rules
        //
        //----------------------------------------------------------------------
        RULE &ID      = term("ID", "[:word:]+");
        RULE &STRING  = plug<Lexical::jString>("STRING");
        RULE &CHARS   = plug<Lexical::rString>("CHARS");
        RULE &RAW     = ( design("RAW") << CHARS << optional( mark('^' ) ) );
        RULE &COLUMN  = mark(':');

        AGG &__RULE = agg("RULE");

        __RULE << ID << COLUMN;
        __RULE << oneOrMore( choice(STRING,ID,RAW) );
        __RULE << END;

        dynamo << oneOrMore(__RULE);

        

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

#include "y/fs/local.hpp"

Y_PROGRAM_START()
{
    vfs &fs = local_fs::instance();

    fs.try_remove_file("dynamo.dot");
    fs.try_remove_file("dynamo.png");
    fs.try_remove_file("dynout.dot");
    fs.try_remove_file("dynout.png");

    DynamoCompiler dynamo;
    dynamo->GraphViz("dynamo.dot");

    dynamo.compile( Module::OpenSTDIN() );
    dynamo.ast->GraphViz("dynout.dot");
}
Y_PROGRAM_END()

