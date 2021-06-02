#include "y/jive/language/axiom/terminal.hpp"
#include "y/jive/language/axiom/dull.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{


    Lexeme * create_lexeme(const Context &ctx,
                           const Tag     &tag,
                           size_t        &stm)
    {
        Lexeme *lx = Lexeme::Create(ctx,tag,stm);
        for(size_t i=alea.leq(10);i>0;--i)
        {
            *lx << Char::Acquire(ctx, alea.range<uint8_t>('a','z') );
        }
        return lx;
    }

}

Y_UTEST(lang_axiom)
{
    Y_UTEST_SIZEOF(Jive::xNode);
    const Tag     tag = Tags::Make("term");
    const Context ctx("context");
    size_t        stm = 0;

    std::cerr << "declaring some nodes" << std::endl;
    Language::Terminal A("Standard",Language::Terminal::Standard);
    Language::Terminal B("Univocal",Language::Terminal::Univocal);
    Language::Terminal C("Division",Language::Terminal::Division);
    Language::Dull     D("Internal");

    xTree tree( xNode::Acquire(D) );
    
    tree->leaves().push_back( xNode::Acquire(D) );
    tree->leaves().push_back( xNode::Acquire(A,create_lexeme(ctx,tag,stm)) );
    tree->leaves().push_back( xNode::Acquire(B,create_lexeme(ctx,tag,stm)) );
    tree->leaves().push_back( xNode::Acquire(C,create_lexeme(ctx,tag,stm)) );
    tree->leaves().head->leaves().push(xNode::Acquire(A,create_lexeme(ctx,tag,stm)) );

    tree->graphViz("ltree.dot");

    const size_t sz = tree->save_to("ltree.bin");
    std::cerr << "written=" << sz << std::endl;

    
}
Y_UTEST_DONE()

