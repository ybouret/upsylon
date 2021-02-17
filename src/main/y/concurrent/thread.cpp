
#include "y/concurrent/thread.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{

    namespace concurrent
    {
        thread::  thread(nucleus::thread::call  user_proc,
                         void                  *user_data,
                         const size_t           user_size,
                         const size_t           user_rank) :
        context(user_size,user_rank),
        proc(user_proc),
        args(user_data),
        id(),
        handle( nucleus::thread::launch(*this,aliasing::_(id)) )
        {
            
        }


        thread:: ~thread() throw()
        {
            nucleus::thread::finish(aliasing::_(handle));
            aliasing::_(id) = 0;
        }

    }
}
