#include "y/concurrent/threads.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    static void myfunc( void *data )
    {
        concurrent::threads &crew = *static_cast<concurrent::threads *>(data);
        {
            Y_LOCK(crew.access);
            std::cerr << "in myfunc..." << std::endl;
        }
    }
}

Y_UTEST(threads)
{
    concurrent::threads crew;

    crew.launch(myfunc,&crew);
    

}
Y_UTEST_DONE()
