#include "y/lang/syntax/parser.hpp"
#include "y/utest/run.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Lang;

namespace {

    class myParser : public Syntax::Parser
    {
    public:
        inline myParser() : Syntax::Parser("JSONLite")
        {
            setVerbose(true);

            ALT   & JSON    = alternate("JSON");
            ALT   & VALUE   = alternate("value");
            TERM  & COMA    = term(',').sm();
            RULE  & STRING  = plug<Lexical::jString>("string");
            {
                AGG & ARRAY = aggregate("array");
                {
                    ARRAY << mark('[');
                    ARRAY << VALUE;
                    ARRAY << zeroOrMore( join(COMA,VALUE) );
                    ARRAY << mark(']');
                }
                JSON << ARRAY;
                VALUE << ARRAY;
            }

            {
                AGG & OBJECT = aggregate("object");
                {
                    OBJECT << mark('{');
                    AGG & PAIR = aggregate("pair");
                    PAIR << STRING << term(':').sm() << VALUE;

                    OBJECT << PAIR;
                    OBJECT << zeroOrMore( join(COMA,PAIR) );
                    OBJECT << mark('}');
                }
                JSON << OBJECT;
                VALUE << OBJECT;
            }

            VALUE << term("null") << term("true","true") << term("false") << term("number","-?[:digit:]+([.][:digit:]+)?") << STRING;


            // lexical only
            (**this).drop("[:blank:]+");
            (**this).endl("[:endl:]");

            end();

            graphViz( *name + ".dot" );
        }

        inline virtual ~myParser() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(myParser);
    };

}

Y_UTEST(parser)
{
    Syntax::Parser::Pointer P = new myParser();
    std::cerr << "Building [" << P->key() << "]" << std::endl;
    if( argc>1 && 0 == strcmp("run",argv[1]) )
    {
        Source source( Module::OpenSTDIN() );
        auto_ptr<Syntax::Node> ast = P->run( source );
        ast->graphViz( *(P->name) + "_tree.dot" );

        const string binfile =*(P->name) + "_tree.bin";
        ast->save_to(binfile);

        auto_ptr<Syntax::Node> reloaded = Syntax::Node::Load( Module::OpenFile(binfile),*P);
        const string ast64 = ast->to_base64();
        const string bin64 = reloaded->to_base64();
        std::cerr << "ast: " << ast64 << std::endl;
        std::cerr << "bin: " << bin64 << std::endl;
        Y_CHECK(ast64==bin64);

        Syntax::Analyzer a;
        a.walk(*ast);

    }
}
Y_UTEST_DONE()


