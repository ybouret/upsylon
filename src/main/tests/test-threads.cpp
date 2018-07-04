#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
#if 0
    static const int count = 8;
    static int       ready = 0;

    static void myfunc( void *data )
    {
        concurrent::threads &crew = *static_cast<concurrent::threads *>(data);
        {
            crew.access.lock();
            ++ready;
            std::cerr << "init myfunc..." << ready << std::endl;
            crew.sync.wait(crew.access);
            std::cerr << "quit myfunc..." << std::endl;

            --ready;
            crew.access.unlock();
        }
    }
#endif
}

Y_UTEST(threads)
{
    std::cerr << "sizeof(thread)=" << sizeof(concurrent::thread) << std::endl;

    concurrent::threads threads(2);

#if 0
    concurrent::threads crew;
    std::cerr << "Starting..." << std::endl;
    for(int i=0;i<count;++i)
    {
        crew.launch(myfunc,&crew);
    }

    while(true)
    {
        if( crew.access.try_lock() )
        {
            if(ready<count)
            {
                crew.access.unlock();
            }
            else
            {
                crew.access.unlock();
                break;
            }
        }
    }

    std::cerr << "back in main" << std::endl;
    crew.sync.broadcast();

    while(true)
    {
        if( crew.access.try_lock() )
        {
            if(ready>0)
            {
                crew.access.unlock();
            }
            else
            {
                crew.access.unlock();
                break;
            }
        }
    }
#endif
}
Y_UTEST_DONE()
