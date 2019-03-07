#include "y/lang/dynamo/parser.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(dyn)
{
    DynamoParser dynamo;
    dynamo.graphViz("dynamo.dot");


    if(argc>1)
    {
        auto_ptr<Syntax::Node> g = dynamo.run( Module::OpenFile(argv[1]) );
    }
}
Y_UTEST_DONE()

