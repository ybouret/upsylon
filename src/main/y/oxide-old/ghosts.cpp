#include "y/oxide/ghosts.hpp"


namespace upsylon
{
    namespace Oxide
    {
        const char * GhostsInfo:: Kind(const bool async) throw()
        {
            return async ? "[async]" : "[local]";
        }


        const char * GhostsComm:: ToText(const unsigned comm) throw()
        {
            switch(comm)
            {
                case None: return "node";
                case Send: return "send";
                case Recv: return "recv";
                case Both: return "both";
                default:
                    break;
            }
            return "????";
        }
    }
}

