
#include "y/lang/compiler.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

#include "y/utest/run.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    class Eval : public Syntax::Parser
    {
    public:
        Eval() : Syntax::Parser("evaluator")
        {


            RULE &ID     = term("ID", "[:alpha:]+");
            RULE &NUM    = term("NUM", "[:digit:]+");
            Alt  &ATOM   = alt("atom");

            Agg  &mulExpr = agg("mulExpr");
            mulExpr << ATOM    << zeroOrMore( agg("extraMul") << term("MULOP","[*/]") << ATOM );
            Agg &addExpr = agg("addExpr");
            addExpr << mulExpr << zeroOrMore( agg("extraAdd") << term("ADDOP","[-+]") << mulExpr );


            ATOM << ID << NUM;
            Agg &eval = agg("eval");
            eval << zeroOrMore( agg("statement") << addExpr << mark(';'));
            top(eval);

            root.endl("endl","[:endl:]");
            root.drop("ws","[:blank:]");
        }

        virtual ~Eval() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Eval);
    };

    class Evaluator : public Compiler
    {
    public:
        Evaluator() :  Compiler( new Eval()  )
        {
        }

        virtual ~Evaluator() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Evaluator);
    };

}

Y_UTEST(eval)
{
    Evaluator ev;

    if(argc>1)
    {
        const string filename = argv[1];
        if("NULL"==filename) return 0;
        ev.compile( Module::OpenFile(filename) );
    }
    else
    {
        ev.compile( Module::OpenSTDIN() );
    }
}
Y_UTEST_DONE()




