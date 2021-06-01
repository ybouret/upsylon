
#include "y/jive/language/grammar.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{


}

Y_UTEST(lang_grammar)
{
    Language::Grammar G("G");

    const Language::Terminal &ID  = G.term("ID", Language::Terminal::Standard);
    const Language::Axiom    &INT = G.term("INT",Language::Terminal::Standard);

    Y_CHECK(G.query("ID"));
    Y_CHECK(G.query("INT"));
    G.setRoot(ID);  Y_CHECK(&ID==G.getRoot());
    G.setRoot(INT); Y_CHECK(&INT==G.getRoot());
}
Y_UTEST_DONE()

