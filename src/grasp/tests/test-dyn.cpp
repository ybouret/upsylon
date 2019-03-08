#include "y/lang/dynamo/parser.hpp"
#include "y/utest/run.hpp"
#include "y/lang/syntax/analyzer.hpp"

using namespace upsylon;
using namespace Lang;

static inline Module * doOpen( const string &fn )
{
    return (fn=="STDIN") ? Module::OpenSTDIN() : Module::OpenFile(fn);
}


Y_UTEST(dyn)
{
    DynamoParser dynamo;
    dynamo.graphViz("dynamo.dot");


    if(argc>1)
    {
        const string           fn = argv[1];

        if( true )
        {
            Source source( doOpen(fn) );
            Lexeme *lx = NULL;
            while( NULL != (lx=dynamo.get(source) ) )
            {
                std::cerr << "<" << lx->label << ">='" << *lx << "'" << std::endl;
                delete lx;
            }
        }

        if(false)
        {
            auto_ptr<Syntax::Node> g  = dynamo.run( doOpen(fn) );
            g->graphViz( "dynamo_tree.dot" );

            Syntax::Analyzer a;
            a.walk(*g);
        }
    }
}
Y_UTEST_DONE()

