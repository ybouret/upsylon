#include "y/jargon/pattern/dictionary.hpp"
#include "y/jargon/pattern/all.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(jargon_types)
{
    std::cerr << "I/O" << std::endl;
    Y_UTEST_SIZEOF(Context);
    Y_UTEST_SIZEOF(Char);
    Y_UTEST_SIZEOF(Token);
    Y_UTEST_SIZEOF(Source);
    Y_UTEST_SIZEOF(Module);
    Y_UTEST_SIZEOF(Char::Pool);
    std::cerr << std::endl;
    
    Y_UTEST_SIZEOF(Pattern);
    std::cerr << std::endl;

    std::cerr << "Basic" << std::endl;
    Y_UTEST_SIZEOF(Basic);
    Y_UTEST_SIZEOF(Any1);
    Y_UTEST_SIZEOF(Single);
    Y_UTEST_SIZEOF(Range);
    Y_UTEST_SIZEOF(Excluded);
    std::cerr << std::endl;

    std::cerr << "Logic" << std::endl;
    Y_UTEST_SIZEOF(Logical);
    Y_UTEST_SIZEOF(AND);
    Y_UTEST_SIZEOF(OR);
    Y_UTEST_SIZEOF(NONE);
    std::cerr << std::endl;
    
    std::cerr << "Joker" << std::endl;
    Y_UTEST_SIZEOF(Joker);
    Y_UTEST_SIZEOF(Optional);
    Y_UTEST_SIZEOF(Repeating);
    Y_UTEST_SIZEOF(Counting);
    std::cerr << std::endl;
    
}
Y_UTEST_DONE()

