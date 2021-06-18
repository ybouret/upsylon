#include "y/jive/dialect/parser.hpp"
#include "y/jive/language/analyzer.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(dialect)
{
    Language::Axiom::Verbose = true;
    Dialect::Parser d;

    Language::Analyzer analyze(d.name);

    if(argc>1)
    {
        XTree  tree( d.parseFile(argv[1]) );
        if( tree.is_valid() )
        {
            tree->graphViz("tree.dot");
            tree->save_to("tree.bin");
            XTree load( XNode::Load( Module::OpenFile("tree.bin"),d) );
            Y_CHECK( load->to_binary() == tree->to_binary() );
            analyze.walk(tree.content());
        }
    }
}
Y_UTEST_DONE()


