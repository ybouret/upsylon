#include "y/ink/engine.hpp"


namespace upsylon
{
    namespace ink
    {
        engine:: ~engine() throw()
        {
        }

        engine:: engine( const dispatcher &shared, const area &full ) :
        area(full),
        agent(shared),
        zones( *this, agent->engine().num_threads() )
        {
        }

    }
}
