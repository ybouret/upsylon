#include "y/oxide/ghosts.hpp"


namespace upsylon
{
    namespace Oxide
    {
        const char * GhostsInfo:: Kind(const bool async) throw()
        {
            return async ? "[async]" : "[local]";
        }
    }
}

