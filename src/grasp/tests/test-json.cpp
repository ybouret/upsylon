
#include "y/lang/syntax/parser.hpp"
#include "y/utest/run.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Lang;

namespace {

    class jParser : public Syntax::Parser
    {
    public:
        inline jParser() : Syntax::Parser("JSON")
        {
            //setVerbose(true);
            
            //__________________________________________________________________
            //
            // 
            //__________________________________________________________________
            ALT  &VALUE  = alternate("value");
            RULE &COMA   = mark(',');
            RULE &STRING = hook<Lexical::jString>("string");
            
            //__________________________________________________________________
            //
            // arrays
            //__________________________________________________________________
            ALT &ARRAY = alternate("array");
            {
                RULE &LBRACK = mark('[');
                RULE &RBRACK = mark(']');
                RULE  &EMPTY_ARRAY = aggregate("empty_array") << LBRACK << RBRACK;
                RULE  &HEAVY_ARRAY = aggregate("heavy_array") << LBRACK << VALUE << zeroOrMore( join(COMA,VALUE) ) << RBRACK;
                ARRAY << EMPTY_ARRAY << HEAVY_ARRAY;
            }
            VALUE << ARRAY;
            
            //__________________________________________________________________
            //
            // objects
            //__________________________________________________________________
            ALT &OBJECT = alternate("object");
            {
                RULE &LBRACE       = mark('{');
                RULE &RBRACE       = mark('}');
                RULE &EMPTY_OBJECT = aggregate("empty_object") << LBRACE << RBRACE;
                RULE &PAIR         = aggregate("pair") << STRING << mark(':') << VALUE;
                RULE &HEAVY_OBJECT = aggregate("heavy_object") << LBRACE << PAIR << zeroOrMore( join(COMA,PAIR) ) << RBRACE;
                OBJECT << EMPTY_OBJECT << HEAVY_OBJECT;
            }
            VALUE << OBJECT;
            
            //__________________________________________________________________
            //
            // finalize value
            //__________________________________________________________________
            VALUE
            <<  term("null")
            <<  term("true")
            <<  term("false")
            <<  term("number","-?[:digit:]+([.][:digit:]+)?")
            << STRING;
            
            
            //__________________________________________________________________
            //
            // lexical only
            //__________________________________________________________________
            (**this).drop("[:blank:]+");
            (**this).endl("[:endl:]");
            
            topLevel(choice(ARRAY,OBJECT));
            end();
            
            graphViz( *name + ".dot" );
        }

        inline virtual ~jParser() throw()
        {

        }

    };

}

Y_UTEST(json)
{
    Syntax::Parser::Pointer P = new jParser();
    std::cerr << "Building [" << P->key() << "]" << std::endl;
    if( argc>1 && 0 == strcmp("run",argv[1]) )
    {
        P->setVerbose(false);
        Source source( Module::OpenSTDIN() );
        auto_ptr<Syntax::Node> ast = P->run( source );
        ast->graphViz( *(P->name) + "_tree.dot" );

        const string binfile = *(P->name) + "_tree.bin";
        ast->save(binfile);

        auto_ptr<Syntax::Node> reloaded = Syntax::Node::Load( Module::OpenFile(binfile),*P);
        const string ast64 = ast->toBase64();
        const string bin64 = reloaded->toBase64();
        //std::cerr << "ast: " << ast64 << std::endl;
        //std::cerr << "bin: " << bin64 << std::endl;
        Y_CHECK(ast64==bin64);

        Syntax::Analyzer a;
        a.walk(*ast);

    }
}
Y_UTEST_DONE()
