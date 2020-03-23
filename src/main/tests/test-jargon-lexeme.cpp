
#include "y/jargon/pattern/regexp.hpp"
#include "y/jargon/lexeme.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jargon;

namespace {
    
}

Y_UTEST(jargon_lexeme)
{
    Y_UTEST_SIZEOF(Lexeme);
    Y_UTEST_SIZEOF(Lexemes);

}
Y_UTEST_DONE()

