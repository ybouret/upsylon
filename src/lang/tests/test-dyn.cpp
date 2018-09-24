#include "y/lang/dynamo/parser.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(dyn)
{
    vfs &fs = local_fs::instance();

    fs.try_remove_file("dynamo.dot");
    fs.try_remove_file("dynamo.png");
    fs.try_remove_file("dynout.dot");
    fs.try_remove_file("dynout.png");

    DynamoParser    dynamo;

    dynamo.GraphViz("dynamo.dot");

    if(argc>1)
    {
        auto_ptr<Lang::Syntax::Node> ast = 0;
        {
            Lang::Source source( Lang::Module::OpenFile(argv[1]) );
            ast = dynamo.parse(source,false);
        }

        ast->GraphViz("dynout.dot");

        Lang::Syntax::Analyzer A;
        A.walk( *ast );
    }

}
Y_UTEST_DONE()

