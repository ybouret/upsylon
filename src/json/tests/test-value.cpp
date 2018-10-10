#include "y/json/compiler.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(value)
{

    if( argc > 1 )
    {
        const string filename = argv[1];
        JSON::Value  j;
        Module      *module   = (filename=="run") ? Module::OpenSTDIN() : Module::OpenFile(filename);
        JSON::Load::From(module,j);
    }

}
Y_UTEST_DONE()

