#include "y/program.hpp"
#include "y/lang/dynamo/generator.hpp"
#include "y/lang/dynamo/compiler.hpp"
#include "y/ios/bin2dat.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;
using namespace Lang;

Y_PROGRAM_START()
{
    vfs            &fs = local_fs::instance();
    DynamoCompiler  dynCompiler;
    DynamoGenerator dynGenerator;

#if 0
    fs.try_remove_file("dynamo.dot");
    fs.try_remove_file("dynamo.png");
    fs.try_remove_file("dynout.dot");
    fs.try_remove_file("dynout.png");

    Lang::DynamoParser    dynamo;

    dynamo.GraphViz("dynamo.dot");

    auto_ptr<Lang::Syntax::Node> ast = 0;
    {
        Lang::Source source( Lang::Module::OpenSTDIN() );
        ast = dynamo.parse(source,false);
    }

    ast->GraphViz("dynout.dot");

    Lang::Syntax::Analyzer A;
    A.walk( *ast );
#endif


    if(argc>1)
    {
        //______________________________________________________________________
        //
        // creating the corresponding parser
        //______________________________________________________________________
        const string             grammarFile = argv[1];
        auto_ptr<Syntax::Node>   grammarAST  = dynCompiler.process( Module::OpenFile(grammarFile) );
        grammarAST->GraphViz("dynast.dot");
        auto_ptr<Syntax::Parser> parser = dynGenerator.create(*grammarAST);
        parser->GraphViz("dynout.dot");
        std::cerr << "Terminals: " << dynGenerator.terminals << std::endl;
        std::cerr << "Internals: " << dynGenerator.internals << std::endl;

        //______________________________________________________________________
        //
        // saving the binary data
        //______________________________________________________________________
        {
            const string grammarBin = grammarAST->to_binary();
            string       grammarInc = grammarFile; vfs::change_extension(grammarInc, "inc");
            std::cerr << "-- saving into '" << grammarInc << "'" << std::endl;
            fs.try_remove_file(grammarInc);
            {
                ios::ocstream fp(grammarInc);
                ios::bin2dat  b2d(16);
                b2d.write(fp,grammarBin);
            }
        }
    }


}
Y_PROGRAM_END()

