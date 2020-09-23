#include "y/jive/syntax/axiom/terminal.hpp"
#include "y/jive/syntax/axiom/internal.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(axiom)
{
    Syntax::Terminal term("word");
    Syntax::Dull     dull("dull");

    Y_UTEST_SIZEOF(Syntax::Node);
    Y_UTEST_SIZEOF(Syntax::Node::List);
    Y_UTEST_SIZEOF(Syntax::Axiom);
    Y_UTEST_SIZEOF(Syntax::Terminal);

    Syntax::Node::List L;
    L.push_back( Syntax::Node::Acquire(term,NULL) );
    L.push_back( Syntax::Node::Acquire(dull) );
}
Y_UTEST_DONE()

