#include "y/seem/compiler.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(compiler)
{
    SEEM::Compiler seem(true);
    //seem->GraphViz("seem.dot");
    if(argc>1)
    {
        const string  filename = argv[1];
        Lang::Module *m   = 0;
        if(filename=="run")
        {
            m = Lang::Module:: OpenSTDIN();
        }
        else
        {
            m = Lang::Module:: OpenFile(filename);
        }
        seem.compile(m);
        seem.ast->GraphViz( "seem_ast.dot" );
    }
}
Y_UTEST_DONE()

