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
    
    if(argc>1)
    {
        auto_ptr<XNode> xnode = dialect.parseFile(argv[1]);
        Y_CHECK(xnode.is_valid());
        xnode->graphViz("dialect_tree.dot");
        xnode->save_to("dialect.bin");
        xnode = dialect.checkIncludes( xnode.yield(), argv[1] );
        
    }
    
}
Y_UTEST_DONE()


