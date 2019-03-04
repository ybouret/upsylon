
#include "y/lang/syntax/parser.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

namespace {

    class myParser : public Syntax::Parser
    {
    public:
        inline myParser() : Syntax::Parser("EE")
        {
            setVerbose(true);
            AGG & EXPR = aggregate("expr");

            AGG & ADD  = aggregate("add");
            AGG & MUL  = aggregate("mul");
            AGG & POW  = aggregate("pow");
            ALT & ATOM = alternate("atom");
            ATOM << term("ID","[:alpha:]+") << term("NUMBER","[:digit:]+") << join( mark('('), ADD, mark( ')' ) );

            ADD << MUL << zeroOrMore( join( term("ADDOP","[-+]" ), MUL ) );
            MUL << POW << zeroOrMore( join( term("MULOP","[*/%]"), POW ) );

            POW << ATOM << optional( join( term('^'), POW) );

            MUL.will(Syntax::MergeOne);
            POW.will(Syntax::MergeOne);
            ADD.will(Syntax::MergeOne);

            EXPR << zeroOrMore( join(ADD,mark(';')) );

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

}

Y_UTEST(ee)
{
    myParser P;
    if( argc>1 && 0 == strcmp("run",argv[1]))
    {
        Source source( Module::OpenSTDIN() );
        auto_ptr<Syntax::Node> ast = P.run( source );
        ast->graphViz( *(P.name) + "_ast.dot" );
    }
}
Y_UTEST_DONE()

