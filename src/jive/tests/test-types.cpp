#include "y/jive/language/parser.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(types)
{
    std::cerr << "Lexical:" << std::endl;
    Y_UTEST_SIZEOF(Token);
    Y_UTEST_SIZEOF(Lexeme);
    Y_UTEST_SIZEOF(Lexer);

    std::cerr << "Language:" << std::endl;
    Y_UTEST_SIZEOF(Language::Node);
    Y_UTEST_SIZEOF(Language::Axiom);
    Y_UTEST_SIZEOF(Language::Terminal);
    Y_UTEST_SIZEOF(Language::Internal);
    Y_UTEST_SIZEOF(Language::Wildcard);
    Y_UTEST_SIZEOF(Language::Repeat);
    Y_UTEST_SIZEOF(Language::Option);
    Y_UTEST_SIZEOF(Language::Compound);
    Y_UTEST_SIZEOF(Language::Aggregate);
    Y_UTEST_SIZEOF(Language::Alternate);
    Y_UTEST_SIZEOF(Language::TermLedger);

    std::cerr << "Parsing:" << std::endl;
    Y_UTEST_SIZEOF(Language::Observer);
    Y_UTEST_SIZEOF(Language::AggNode);
    Y_UTEST_SIZEOF(Language::AggPool);

    Y_UTEST_SIZEOF(Language::Grammar);
    Y_UTEST_SIZEOF(Language::Parser);



}
Y_UTEST_DONE()

