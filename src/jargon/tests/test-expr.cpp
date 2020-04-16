
#include "y/jargon/parser.hpp"
#include "y/utest/run.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/evaluator.hpp"
#include "y/ios/serialized.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
    class Expr : public Parser
    {
    public:
        explicit Expr() : Parser("Expr")
        {
            
            // declare terminals
            Aggregate &ADDOP = act("ADDOP");
            
            Alternate &ATOM = alt();
            ATOM
            << term("ID",RegularExpression::Identifier)
            << term("INT","[:digit:]+")
            << cat( mark('('), ADDOP, mark(')') );
            
            Aggregate &POWER = act("POW");
            POWER << ATOM << opt( cat(mark('^'), POWER) );
            
            Aggregate &MULOP = act("MULOP");
            MULOP << POWER << zom( cat( choice( op('*'), op('/') ), POWER ) );
            
            Axiom &PM = op("PM","[-+]"); //choice( op('+'), op('-') );
            ADDOP << opt(PM) << MULOP << zom( cat( PM , MULOP ) );
            
            endl("endl", "[:endl:]");
            drop("blank","[:blank:]");
            
            graphViz("expr.dot");
            compile();
        }
        
        
        virtual ~Expr() throw()
        {}
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Expr);
    };
}

Y_UTEST(expr)
{
    //Axiom::Verbose = true;
    Expr expr;
    if(argc>1 )
    {
        //expr.buildAST = false;
        const string    fileName = argv[1];
        auto_ptr<XNode> tree = expr.parseFile(fileName);
        
        tree->graphViz("expr_tree.dot");
        tree->save_to("expr_tree.bin");
        {
            auto_ptr<XNode> tree2 = expr.loadTreeFromFile("expr_tree.bin");
            Y_CHECK(ios::serialized::are_same_binary(*tree,*tree2));
        }
        
        
        Evaluator_ E(expr.title);
        E.browse(*tree);
        
        std::cerr << "done" << std::endl;
    }
}
Y_UTEST_DONE()
