
#include "y/jive/syntax/axiom/terminal.hpp"
#include "y/jive/syntax/axiom/internal.hpp"
#include "y/jive/syntax/grammar.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
 
using namespace upsylon;
using namespace Jive;

Y_UTEST(grammar)
{
    Syntax::Grammar G("G");
    
    Syntax::Terminal &term = G.add( new Syntax::Terminal("term") );
    Syntax::Internal &dull = G.add( new Syntax::Dull("dull") );

    XNode::Pointer root( XNode::Acquire(dull) );
    root->leaves().push_back( XNode::Acquire(term,NULL) );
    root->leaves().push_back( XNode::Acquire(dull) );
    
    root->save_to("dull.bin");
    {
        Source         temp( Module::OpenFile("dull.bin") );
        XNode::Pointer load( XNode::Load(temp,G) );
        Y_CHECK(load.is_valid());
    }
    
}
Y_UTEST_DONE()

