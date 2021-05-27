#include "y/csv/parser.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/endl.hpp"

using namespace upsylon;

static inline void createCSV(const char *fileName)
{
    ios::ocstream fp(fileName);
    
    fp << "hello,world" << ios::crlf;
    
    
}

Y_UTEST(CSV)
{
    Jive::Syntax::Axiom::Verbose = true;
    CSV::Parser          parser;
    createCSV("sample.csv");
    
    if(argc>1)
    {
        Jive::Source               source( Jive::Module::OpenFile(argv[1]));
        const Jive::XNode::Pointer tree( parser.parse(source) );
        tree->graphViz("csv.dot");
    }
}
Y_UTEST_DONE()

