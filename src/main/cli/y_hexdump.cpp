#include "y/program.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;
static size_t   width = 16;

static inline
void display(  string &line, ios::ostream &fp )
{
    assert(line.size()<=width);
    const size_t n = line.size();
    // prolog

    // bulk
    for(size_t i=0;i<n;++i)
    {
        uint8_t C = uint8_t(line[i]);
        fp << ' ' << hexadecimal::lowercase[ C ];
        if(C<32||C>=127) C = '.';
        line[i] = C;
    }
    for(size_t i=n;i<width;++i)
    {
        fp << "   ";
        line << ' ';
    }
    // epilog
    fp << ' ' << '|' << line << '|' << '\n';
}

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
    size_t count = 0;
    while( inp->query(C) )
    {
        ++count;
        line << C;
        if(line.size()>=width)
        {
            display(line,*out);
            line.clear();
        }
    }
    if(line.size()>0)
    {
        display(line,*out);
    }
    out->flush();




}
Y_PROGRAM_END()

