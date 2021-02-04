#include "y/utest/run.hpp"

#if defined(Y_BSD)
#include <sys/mman.h>
#else

#endif

using namespace upsylon;


Y_UTEST(mlock)
{

    int block_size = 1024;
    if(argc>1)
    {
        block_size = atoi(argv[1]);
    }

#if defined(Y_BSD)
    std::cerr << "+" << block_size << std::endl;
    void * addr = calloc(block_size,1);
    if(addr)
    {
        const int status = mlock(addr,block_size);
        if(status!=0)
        {
            perror("mlock");
        }
        else
        {
            munlock(addr,block_size);
        }
        free(addr);
    }
    else
    {
        perror("no memory");
    }
#endif

}
Y_UTEST_DONE()

