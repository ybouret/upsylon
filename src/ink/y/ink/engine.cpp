#include "y/ink/engine.hpp"


namespace upsylon
{
    namespace Ink
    {
        Engine:: ~Engine() throw()
        {
        }

        Engine:: Engine( const Dispatcher &shared, const area &full ) :
        area(full),
        agent(shared),
        zones( *this, agent->engine().num_threads() )
        {
        }

        void engine:: acquire_all( const size_t n )
        {
            for(size_t i=zones.size();i>0;--i)
            {
                zones[i].cache.acquire(n);
            }
        }
    }
}
