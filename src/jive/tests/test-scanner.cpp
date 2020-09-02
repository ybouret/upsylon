#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/lexical/scanner.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(scanner)
{
    Y_UTEST_SIZEOF(Lexical::Rule);
    Y_UTEST_SIZEOF(Lexical::Scanner);
    
}
Y_UTEST_DONE()
