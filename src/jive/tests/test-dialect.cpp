#include "y/jive/dialect/loader.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local/fs.hpp"
#include "y/jive/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(dialect)
{
    Dialect::Loader  dialect;
    Syntax::Analyzer A(  dialect.name );

    Syntax::Axiom::Verbose = true;
    
    vfs &fs = local_fs::instance();
    fs.try_remove_file("dialect.dot");
    fs.try_remove_file("dialect.png");
    
    //Tags::Display();
    std::cerr << dialect.getRegistry() << std::endl;
    if(argc>1)
    {
        XNode::Pointer ast( dialect.loadSketch(argv[1]) );

        ast->graphViz("dialect.dot");
        std::cerr << "Analyzing" << std::endl;
        A.walk( ast.content() );
        std::cerr << std::endl;

        {
            ast->save_to("dialect.bin");
            XNode::Pointer ast2( dialect.loadBinary( Module::OpenFile("dialect.bin") ) );
            std::cerr << "Analyzing reloaded" << std::endl;
            A.walk( ast2.content() );
            std::cerr << std::endl;
        }


    }

}
Y_UTEST_DONE()
