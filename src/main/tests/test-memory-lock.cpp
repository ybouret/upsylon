
#include "y/os/page-size.hpp"
#include "y/memory/locked.hpp"
#include "y/utest/run.hpp"




using namespace upsylon;


Y_UTEST(mlock)
{


	int block_size = 5000;
	if (argc > 1)
	{
		block_size = atoi(argv[1]);
	}

    std::cerr << "page_size  = " << page_size::get() << std::endl;
    std::cerr << "block_size = " << block_size << std::endl;
    memory::locked_area blk( block_size );
    std::cerr << "blk.bytes  = " << blk.bytes << std::endl;

    char  *      s = (char *)(blk.entry);
    const size_t n = alea.lt(blk.bytes);
    for(size_t i=0;i<n;++i)
    {
        s[i] = alea.range<char>('a','z');
    }
    



}
Y_UTEST_DONE()

