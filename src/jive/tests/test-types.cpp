#include "y/jive/syntax/node.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(types)
{
    Y_UTEST_SIZEOF(Char);
    Y_UTEST_SIZEOF(Lexeme);
    Y_UTEST_SIZEOF(XNode);
    Y_CHECK(Char::Supply::instance().block_size == sizeof(Char));
    Y_CHECK(Lexeme::Supply::instance().block_size == sizeof(Lexeme));
    Y_CHECK(XNode::Supply::instance().block_size == sizeof(XNode));

}
Y_UTEST_DONE()

