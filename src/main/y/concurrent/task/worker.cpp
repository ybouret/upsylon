
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
            crew.access.lock();
            std::cerr << "initialized worker@" << label << std::endl;
            ++crew.ready;
            wait(crew.access);

            std::cerr << "done worker@" << label << std::endl;
            crew.access.unlock();
        }

        
    }
}

namespace upsylon
{
    namespace concurrent
    {

        team:: ~team() throw()
        {
            {
                Y_LOCK(access);
                std::cerr << "team.quit" << std::endl;
            }
            while( waiting.size )
            {
                worker *w = waiting.pop_back();
                w->broadcast();
                delete w;
            }

        }

        team:: team() :
        waiting(),
        ready(0)
        {

            setup();
        }

        
        void team:: setup()
        {
            try
            {
                const size_t  count = topo->size();
                const size_t &rank  = waiting.size;
                while(rank<count)
                {
                    waiting.push_back( new worker(*this,count,rank) );
                }
                Y_MUTEX_PROBE(access,ready>=count);
                std::cerr << "synchronized" << std::endl;

            }
            catch(...)
            {
                // emergency exit
                while( waiting.size )
                {
                    worker *w = waiting.pop_back();
                    w->broadcast();
                    delete w;
                }
                throw;
            }
        }

        void team::call(const context & )
        {

        }

    }

}

