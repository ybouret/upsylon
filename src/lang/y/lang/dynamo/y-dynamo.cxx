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

    Lang::Dynamo::Generator dynamo;
    dynamo->GraphViz("dynamo.dot");

    dynamo.compile( Lang::Module::OpenSTDIN() );
    dynamo.ast->GraphViz("dynout.dot");
    

}
Y_PROGRAM_END()

