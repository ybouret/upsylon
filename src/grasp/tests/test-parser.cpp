#include "y/lang/syntax/parser.hpp"
#include "y/utest/run.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

using namespace upsylon;
using namespace Lang;

class myParser : public Syntax::Parser
{
public:
    inline myParser() : Syntax::Parser("JSONLite")
    {
        setVerbose(true);

        ALT  & JSON    = alternate("JSON");
        ALT   & VALUE  = alternate("value");
        TERM  & COMA   = term(',').setSemantic();
        RULE & STRING  = hook<Lexical::jString>("string");
        {
            AGG & ARRAY = aggregate("array");
            ARRAY << mark('[');
            ARRAY << VALUE;
            ARRAY << zeroOrMore( join(COMA,VALUE) );
            ARRAY << mark(']');
            VALUE << ARRAY;

            JSON << ARRAY;
        }

        {
            AGG & OBJECT = aggregate("object");
            {
                OBJECT << mark('{');
                AGG & PAIR = aggregate("pair");
                PAIR << STRING << mark(':') << VALUE;

                OBJECT << PAIR;
                OBJECT << zeroOrMore( join(COMA,PAIR) );
                OBJECT << mark('}');
                JSON << OBJECT;
            }
        }

        VALUE << term("null") << term("true") << term("false") << term("number","-?[:digit:]+([.][:digit:]+)?") << STRING;
        
        finalize();

        // lexical only
        (**this).drop("[:blank:]+");
        (**this).endl("[:endl:]");

        graphViz( *name + ".dot" );
    }

    inline virtual ~myParser() throw()
    {
    }

private:
    Y_DISABLE_COPY_AND_ASSIGN(myParser);
};

Y_UTEST(parser)
{
    Syntax::Parser::Pointer P = new myParser();
    std::cerr << "Building [" << P->key() << "]" << std::endl;
    if( argc>1 && 0 == strcmp("run",argv[1]))
    {
        Source source( Module::OpenSTDIN() );
        auto_ptr<Syntax::Node> ast = P->run( source );
        ast->graphViz( *(P->name) + "_ast.dot" );
    }
}
Y_UTEST_DONE()


