#include "y/lang/dynamo/compiler.hpp"
#include "y/lang/dynamo/generator.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local.hpp"
#include "y/codec/lzo.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(dyn)
{
    vfs &fs = local_fs::instance();

    fs.try_remove_file("dynamo.dot");
    fs.try_remove_file("dynamo.png");
    fs.try_remove_file("dynout.dot");
    fs.try_remove_file("dynout.png");
    fs.try_remove_file("dyn1.bin");
    fs.try_remove_file("dyn2.bin");


    DynamoCompiler    dynamo;

    dynamo.parser.GraphViz("dynamo.dot");

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
            ios::ocstream fp("dyn1.bin");
            ast->save(fp);
        }

        {
            const string bst = ast->to_binary();
            {
                ios::ocstream fp("dyn2.bin");
                fp << bst;
            }
            {
                ios::ocstream fp("dyn3.bin");
                miniLZO::instance().Compress(fp,bst);
            }
        }

        Lang::Syntax::Analyzer A;
        A.walk( *ast );

        DynamoGenerator gen;
        auto_ptr<Lang::Syntax::Parser> parser = gen.create( *ast, true );

        ast->GraphViz("dynclr.dot");

        std::cerr << "sizeof(DynamoParser)    = " << sizeof(DynamoParser)    << std::endl;
        std::cerr << "sizeof(DynamoCompiler)  = " << sizeof(DynamoCompiler)  << std::endl;
        std::cerr << "sizeof(DynamoGenerator) = " << sizeof(DynamoGenerator) << std::endl;

    }

}
Y_UTEST_DONE()

