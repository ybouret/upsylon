#include "y/jargon/dialect.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(dialect)
{
    Lexical::Scanner::Verbose = true;
    Axiom::Verbose            = true;
    Jargon::Dialect dialect;
    
    dialect.graphViz("dialect.dot");
    dialect.graphVizPatterns("dialect_patterns.dot");
}
Y_UTEST_DONE()


