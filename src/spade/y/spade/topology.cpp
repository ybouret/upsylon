#include "y/spade/topology.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        namespace Kernel
        {

            Topology:: ~Topology() throw()
            {
                _bzset(size);
            }
            
            Topology:: Topology(const size_t nc) :
            size(nc)
            {
                if(size<=0) throw exception("Spade::Topology(no cores)");
            }
            
            Coord1D Topology:: Prev(Coord1D localSize, Coord1D localRank) throw()
            {
                assert(localRank>=0);
                assert(localRank<localSize);
                return (localRank>0) ? --localRank : --localSize;
            }
            
            Coord1D Topology:: Next(Coord1D localSize, Coord1D localRank) throw()
            {
                assert(localRank>=0);
                assert(localRank<localSize);
                return (++localRank>=localSize) ?  0 : localRank;
            }
            
            const Coord1D Coordination<1>::Probes[Directions]= {
                1
            };
            
            const Coord2D  Coordination<2>::Probes[Directions]= {
                Coord2D(1,0),
                Coord2D(0,1),
                
                Coord2D(1,1),
                Coord2D(1,-1)
            };
            
            const Coord3D Coordination<3>::Probes[Directions]= {
                Coord3D(1,0,0),
                Coord3D(0,1,0),
                Coord3D(0,0,1),
                
                Coord3D(1,1,0),
                Coord3D(1,-1,0),
                Coord3D(1,0,1),
                Coord3D(1,0,-1),
                Coord3D(0,1,1),
                Coord3D(0,1,-1),
                
                Coord3D(1,1,1),
                Coord3D(1,1,-1),
                Coord3D(1,-1,-1),
                Coord3D(1,-1,1)
                
            };

        }
        
       
        
    }
    
}

