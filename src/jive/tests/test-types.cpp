#include "y/jive/syntax/axiom/all.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(types)
{
    std::cerr << "Base Types" << std::endl;
    Y_UTEST_SIZEOF(Char);
    Y_UTEST_SIZEOF(Lexeme);
    Y_UTEST_SIZEOF(XNode);
    Y_CHECK(Char::Instance().block_size == sizeof(Char));
    Y_CHECK(Lexeme::Instance().block_size == sizeof(Lexeme));
    Y_CHECK(XNode::Instance().block_size == sizeof(XNode));
    std::cerr << std::endl;

    std::cerr << "Axioms" << std::endl;
    Y_UTEST_SIZEOF(Syntax::Axiom);
    Y_UTEST_SIZEOF(Syntax::Terminal);
    Y_UTEST_SIZEOF(Syntax::Aggregate);
    Y_UTEST_SIZEOF(Syntax::Alternate);
    Y_UTEST_SIZEOF(Syntax::Option);
    Y_UTEST_SIZEOF(Syntax::Repeat);



}
Y_UTEST_DONE()

