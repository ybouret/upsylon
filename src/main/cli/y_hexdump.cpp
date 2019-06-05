#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"


using namespace upsylon;
static size_t   width = 16;

Y_PROGRAM_START()
{
    //--------------------------------------------------------------------------
    //
    // select input
    //
    //--------------------------------------------------------------------------
    auto_ptr<ios::icstream> inp = 0;
    switch(argc)
    {
        case 1: inp = new ios::icstream( ios::cstdin ); break;
        case 2: inp = new ios::icstream( argv[1] );     break;
        default: throw exception("usage: %s [file]",program);
    }
    assert(inp.is_valid());

    //--------------------------------------------------------------------------
    //
    // prepare output
    //
    //--------------------------------------------------------------------------
    auto_ptr<ios::ocstream> out = new ios::ocstream( ios::cstdout );
    char   C = 0;
    string line(width,as_capacity);
    while( inp->query(C) )
    {

    }


    out->flush();




}
Y_PROGRAM_END()

