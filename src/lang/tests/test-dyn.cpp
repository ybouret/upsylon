#include "y/lang/dynamo/compiler.hpp"
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

    DynamoCompiler    dynamo;

    dynamo.parser->GraphViz("dynamo.dot");

    if(argc>1)
    {
        const string fn = argv[1];
        auto_ptr<Lang::Syntax::Node> ast = 0;
        {
            Lang::Module *module = 0;
            if(fn=="run")
            {
                module = Lang::Module::OpenSTDIN();
            }
            else
            {
                module = Lang::Module::OpenFile(fn);
            }
            ast = dynamo.process(module);
        }

        ast->GraphViz("dynout.dot");
        {
            ios::ocstream fp("dyn.bin");
            ast->save(fp);
        }

        Lang::Syntax::Analyzer A;
        A.walk( *ast );
    }

}
Y_UTEST_DONE()

