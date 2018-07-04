
#include "y/concurrent/threads.hpp"

namespace upsylon
{
    namespace concurrent
    {
        threads:: ~threads() throw()
        {
        }

        threads:: threads(const size_t n) :
        __threads(n),
        access(),
        synchronize(),
        ready(0)
        {
            for(size_t i=0;i<count;++i)
            {
                build<thread_proc,void*>(start,this);
            }
        }


        void threads:: start( void *args ) throw()
        {
            assert(args);
            static_cast<threads *>(args)->loop();
        }

    }
}

namespace upsylon
{
    namespace concurrent
    {
        void threads:: loop() throw()
        {
            
        }
    }

}
