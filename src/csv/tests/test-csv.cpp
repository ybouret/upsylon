#include "y/csv/parser.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(CSV)
{
    CSV::Parser parser;
    Jive::Syntax::Axiom::Verbose = true;
    
    if(argc>1)
    {
        Jive::Source               source( Jive::Module::OpenFile(argv[1]));
        const Jive::XNode::Pointer tree( parser.parse(source) );
        tree->graphViz("csv.dot");
    }
}
Y_UTEST_DONE()

