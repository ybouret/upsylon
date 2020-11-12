#include "y/jive/dialect/parser.hpp"
#include "y/utest/run.hpp"
#include "y/fs/local/fs.hpp"
#include "y/jive/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(dialect)
{
    Dialect::Parser dialect;

    Syntax::Axiom::Verbose = true;
    
    vfs &fs = local_fs::instance();
    fs.try_remove_file("dialect.dot");
    fs.try_remove_file("dialect.png");
    
    //Tags::Display();
    std::cerr << dialect.getRegistry() << std::endl;
    if(argc>1)
    {
        XNode::Pointer ast( dialect.parseFile(argv[1]) );
        ast->graphViz("dialect.dot");
        Syntax::Analyzer A(  dialect.name );
        A.walk( ast.content() );
    }

}
Y_UTEST_DONE()
