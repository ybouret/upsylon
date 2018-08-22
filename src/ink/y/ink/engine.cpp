#include "y/ink/engine.hpp"


namespace upsylon
{
    namespace ink
    {
        engine:: ~engine() throw()
        {
        }

        engine:: engine( const workers &shared, const area &full ) :
        area(full),
        agents(shared),
        tiles( *this, agents->engine().num_threads() )
        {
        }

    }
}
