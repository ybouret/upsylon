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


        static const char fn[] = "Oxide::Topology::";

        void Topology:: CheckSameLevels( const Level lhs, const Level rhs )
        {
            if(0==(lhs&Levels)||0==(rhs&Levels) || (lhs!=rhs) )
            {
                throw exception("%sCheckSameLevels(%u,%u) failure",fn,lhs,rhs);
            }
        }

        void Topology:: CheckGhosts( const Coord1D ng, const Coord1D width, const unsigned dim)
        {
            assert(ng>=0);
            if(ng>width) throw exception("%sCheckGhosts(%ld>%ld in dimension#%u)",fn,long(ng),long(width),dim);


        }

    }
}
