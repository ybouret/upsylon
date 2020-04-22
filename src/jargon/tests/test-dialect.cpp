#include "y/jargon/dialect.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(dialect)
{
    Jargon::Dialect dialect;
    
    dialect.graphViz("dialect.dot");
    
}
Y_UTEST_DONE()


