
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
            ALT  &ATOM   = alt("atom");

            AGG  &mulExpr = design("mulExpr");
            mulExpr << ATOM    << zeroOrMore( acting("extraMul") << term("MULOP","[*/]") << ATOM );
            AGG &addExpr = design("addExpr");
            addExpr << mulExpr << zeroOrMore( acting("extraAdd") << term("ADDOP","[-+]") << mulExpr );


            ATOM << ID << NUM;
            //ATOM << mark('(') << addExpr << mark(')');
            AGG &eval = agg("eval");
            eval << zeroOrMore( agg("statement") << addExpr << mark(';'));
            top(eval);

            root.endl("endl","[:endl:]");
            root.drop("ws","[:blank:]");

            checkValidity();
            setVerbose(true);
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

    ev->GraphViz("eval.dot");


    Module *module = 0;
    if(argc>1)
    {
        const string filename = argv[1];
        if("NULL"==filename) return 0;
        module = Module::OpenFile(filename);
    }
    else
    {
        module = Module::OpenSTDIN();
    }

    ev.compile(module,Compiler::KeepAST|Compiler::KeepRaw);
    assert(ev.ast.is_valid());
    ev.ast->GraphViz("eval-ast.dot");
    assert(ev->raw.is_valid());
    ev->raw->GraphViz("eval-raw.dot");


}
Y_UTEST_DONE()




