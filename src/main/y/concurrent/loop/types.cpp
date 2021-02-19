
#include "y/concurrent/loop/types.hpp"

namespace upsylon
{

    namespace concurrent
    {

        looper:: looper() throw()
        {
        }

        looper:: ~looper() throw()
        {
        }


        void looper:: for_each(runnable &obj) throw()
        {
            loop(obj);
            join();
        }




    }

}

#include "y/concurrent/nucleus/thread.hpp"

namespace upsylon
{

    namespace concurrent
    {

        double looper:: efficiency(const double speed_up) const throw()
        {
            return nucleus::thread::efficiency(speed_up,size());
        }


    }

}

