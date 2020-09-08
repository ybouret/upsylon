
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/lexical/editor/program.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Jive;

namespace {


}

Y_UTEST(editor)
{
    Lexical::Editor::Program prog;
    if(argc>1)
    {
        Source        source( Module::OpenFile(argv[1]) );
        ios::ocstream fp( ios::cstdout );

        prog.run(fp,source);
    }
}
Y_UTEST_DONE()

