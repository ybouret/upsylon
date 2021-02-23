
#include "y/concurrent/task/worker.hpp"
#include "y/momentary/value.hpp"

namespace upsylon
{
    namespace concurrent
    {

        worker:: ~worker() throw()
        {
        }

        worker:: worker(team        &user_team,
                        const size_t user_size,
                        const size_t user_rank) :
        object(),
        condition(),
        team::reference(user_team),
        thread(stub,this,user_size,user_rank)
        {

        }

        void worker:: stub(void *addr) throw()
        {
            static_cast<worker *>(addr)->initialize();
        }

        void worker:: initialize() throw()
        {
            Y_LOCK(crew.access);
            
        }

        
    }
}

namespace upsylon
{
    namespace concurrent
    {

        team:: ~team() throw()
        {
        }

        team:: team() :
        waiting()
        {
            try
            {

            }
            catch(...)
            {
                throw;
            }

        }

        
        void team:: setup()
        {

        }


    }

}

