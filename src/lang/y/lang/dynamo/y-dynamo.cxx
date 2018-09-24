#include "y/program.hpp"
#include "y/lang/dynamo/dynamo.hpp"
#include "y/fs/local.hpp"

using namespace upsylon;

Y_PROGRAM_START()
{
    vfs &fs = local_fs::instance();

    fs.try_remove_file("dynamo.dot");
    fs.try_remove_file("dynamo.png");
    fs.try_remove_file("dynout.dot");
    fs.try_remove_file("dynout.png");

    Lang::Dynamo::Parser    dynamo;

    dynamo.GraphViz("dynamo.dot");

    auto_ptr<Lang::Syntax::Node> ast = 0;
    {
        Lang::Source source( Lang::Module::OpenSTDIN() );
        ast = dynamo.parse(source,false);
    }

    ast->GraphViz("dynout.dot");

    Lang::Syntax::Analyzer A;
    A.walk( *ast );
    

}
Y_PROGRAM_END()

