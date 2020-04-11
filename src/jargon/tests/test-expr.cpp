
#include "y/jargon/parser.hpp"
#include "y/utest/run.hpp"

#include "y/jargon/lexical/plugin/end-of-line-comment.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/evaluator.hpp"

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
            MULOP << POWER << zom( cat( choice( term('*'), term('/') ), POWER ) );
            
            Axiom &PM = choice( term('+'), term('-') );
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
    Axiom::Verbose = true;
    Expr expr;
    if(argc>1 )
    {
        const string    fileName = argv[1];
        auto_ptr<XNode> tree = expr.parseFile(fileName);
        
        tree->graphViz("expr_tree.dot");
        
        Evaluator_ E(expr.title);
        E.browse(*tree);
        
        std::cerr << "done" << std::endl;
    }
}
Y_UTEST_DONE()
