
#include "y/concurrent/thread.hpp"
#include "y/exceptions.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

#include <iostream>


namespace upsylon
{

    
    namespace concurrent
    {
        

        thread:: thread(thread_proc  user_proc,
                        void        *user_data,
                        const size_t user_size,
                        const size_t user_rank) :
        parallel(user_size,user_rank),
        proc(user_proc),
        args(user_data),
        id(),
        handle( nucleus::thread::launch(*this,aliasing::_(id)) )
        {
        }

        thread:: ~thread() throw()
        {
            nucleus::thread::finish((nucleus::thread::handle&)handle);
            bzset( (nucleus::thread::ID &)id );
        }

        bool thread:: is_current() const throw()
        {
            const nucleus::thread::ID __curr_id = nucleus:: thread:: get_current_id();
            return nucleus::thread::equal(id,__curr_id);
        }
    }
}


