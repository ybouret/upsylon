#include "y/lang/syntax/grammar.hpp"

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(grammar)
{
    const Tag id = new string("grammar");

    Syntax::Grammar G( id );

    Syntax::Terminal  &ID  = G.decl( new Syntax::Terminal("ID") );
    Syntax::OneOrMore &IDS = G.decl( new Syntax::OneOrMore( "IDS", ID ) );
    std::cerr << "topLevel=" << G.topLevel().name << std::endl;
    G.topLevel(IDS);
    std::cerr << "topLevel=" << G.topLevel().name << std::endl;


}
Y_UTEST_DONE()

