#include "y/bz2++/bz2stream.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/string.hpp"


using namespace upsylon;

Y_UTEST(cat)
{
    
    bzlib::ibz2stream *bzf = 0;
    if( argc > 0 )
        bzf = new bzlib::ibz2stream( argv[1] );
    else
        bzf = new bzlib::ibz2stream( ios::cstdin );
    
    auto_ptr<bzlib::ibz2stream> bz( bzf );
    string line;
    while( bz->gets(line)  )
    {
        std::cout << line << std::endl;
        line.clear();
    }
    
}
Y_UTEST_DONE()
