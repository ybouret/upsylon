
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

            AGG &statement = agg("statement");

            ALT & atom = alt("atom");

            atom << term("ID","[:alpha:]+") << term("INT","[:digit:]+");

            AGG & mulExpr = design("mulExpr");
            mulExpr << atom << zeroOrMore( acting("extraMul") << choice( op('*'), op('/'), op('%') )  << atom );

            AGG & addExpr = design("addExpr");
            addExpr << mulExpr << zeroOrMore( acting("extraAdd") << choice( op('+'), op('-')) << mulExpr );

            atom << ( acting("group") << mark('(') << addExpr << mark (')') );
            statement << zeroOrMore( acting("expr") << addExpr << mark(';') );

            root.endl("endl","[:endl:]");
            root.drop("ws","[:blank:]");

            checkValidity();
            //setVerbose(true);
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
    assert(ev.cst.is_valid());
    ev.cst->GraphViz("eval-cst.dot");

}
Y_UTEST_DONE()




