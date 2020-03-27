#include "y/jargon/axiom.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(types)
{
    Y_UTEST_SIZEOF(XNode);
    Y_UTEST_SIZEOF(XList);
    Y_UTEST_SIZEOF(Lexeme);
    Y_UTEST_SIZEOF(Lexemes);

}
Y_UTEST_DONE()

