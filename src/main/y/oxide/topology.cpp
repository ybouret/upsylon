#include "y/oxide/topology.hpp"
#include "y/exception.hpp"

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



        void Topology:: CheckSameLevels( const Level lhs, const Level rhs )
        {
            if(0==(lhs&Levels)||0==(rhs&Levels) || (lhs!=rhs) )
            {
                throw exception("Oxide::Topology::CheckSameLevels(%u,%u) failure",lhs,rhs);
            }
        }


    }
}
