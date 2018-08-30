#include "y/ink/engine.hpp"


namespace upsylon
{
    namespace Ink
    {
        Engine:: ~Engine() throw()
        {
        }

        Engine:: Engine( const Dispatcher &shared, const Area &full ) :
        Area(full),
        agent(shared),
        tiles( *this, agent->engine().num_threads() )
        {
        }

        void Engine:: acquire_all( const size_t n )
        {
            for(size_t i=tiles.size();i>0;--i)
            {
                tiles[i].cache.acquire(n);
            }
        }
    }
}
