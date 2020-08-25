
#include "y/memory/tight/twain-allocator.hpp"
#include "y/utest/run.hpp"

#include "y/concurrent/fake-lock.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace memory;

Y_UTEST(tight_twain)
{

    size_t chunk_size = 4096;
    size_t limit_size = 128;

    if(argc>1)
    {
        chunk_size = string_convert::to<size_t>(argv[1],"chunk_size");
    }

    if(argc>2)
    {
        limit_size = string_convert::to<size_t>(argv[2],"limit_size");
    }


    concurrent::fake_lock sync;



    tight::twain_allocator twa(sync,chunk_size,limit_size);

    std::cerr << "sizeof(arena)=" << sizeof(tight::arena) << std::endl;
    std::cerr << "sizeof(twain_allocator)=" << sizeof(tight::twain_allocator) << std::endl;
}
Y_UTEST_DONE()

