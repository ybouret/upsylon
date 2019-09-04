#include "y/oxide/topology.hpp"

namespace upsylon
{
    namespace Oxide
    {

        Topology::Level Topology:: LevelFor( const Coord1D *addr, const size_t size ) throw()
        {
            assert(addr);
            assert(size<=3);
            int count = 0;
            for(size_t i=0;i<size;++i)
            {
                if( addr[i] ) ++count;
            }
            switch(count)
            {
                case 1: return Level1;
                case 2: return Level2;
                case 3: return Level3;
                default:
                    break;
            }
            return Level0;
        }

    }
}
