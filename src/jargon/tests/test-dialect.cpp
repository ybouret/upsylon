#include "y/jargon/dialect.hpp"
#include "y/utest/run.hpp"
#include "y/ios/serialized.hpp"

using namespace upsylon;
using namespace Jargon;

static const char demo[] =
".demo;";

Y_UTEST(dialect)
{
    Lexical::Scanner::Verbose = true;
    Axiom::Verbose            = true;
    Jargon::Dialect dialect;
    
    dialect.graphViz("dialect.dot");
    dialect.graphVizPatterns("dialect_patterns.dot");
    
    if(true)
    {
        auto_ptr<XNode> ast = dialect.compileFlat( Module::Open("demo",demo,sizeof(demo)));
        ast->graphViz("demo.dot");
    }
    
    if(argc>1)
    {
        auto_ptr<XNode> xnode = dialect.compileFile(argv[1]);
        xnode->graphViz("dialect_tree.dot");
        xnode->save_to("dialect.bin");
        {
            auto_ptr<XNode> IL = dialect.loadTreeFromFile("dialect.bin");
            Y_CHECK(ios::serialized::are_same_binary(*xnode,*IL));
        }
        
        {
            const string    data = xnode->to_binary();
            auto_ptr<XNode> IL2  = dialect.loadTreeFromData("compiled",data);
            Y_CHECK(ios::serialized::are_same_binary(*xnode,*IL2));
        }
        
        
    }
    
}
Y_UTEST_DONE()


