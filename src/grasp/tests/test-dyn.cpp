#include "y/lang/dynamo/parser.hpp"
#include "y/utest/run.hpp"
#include "y/lang/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(dyn)
{
    DynamoParser dynamo;
    dynamo.graphViz("dynamo.dot");


    if(argc>1)
    {
        const string           fn = argv[1];
        auto_ptr<Syntax::Node> g  = dynamo.run( (fn=="STDIN") ? Module::OpenSTDIN() : Module::OpenFile(argv[1]) );
        g->graphViz( "dynamo_tree.dot" );

        Syntax::Analyzer a;
        a.walk(*g);

    }
}
Y_UTEST_DONE()

