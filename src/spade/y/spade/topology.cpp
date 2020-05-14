#include "y/spade/topology.hpp"
#include "y/exception.hpp"
#include "y/parops.hpp"

namespace upsylon {
    
    namespace Spade
    {
        Connect::Mode Connect::For(const bool exists, const size_t src, const size_t tgt) throw()
        {
            if(exists)
            {
                return (src==tgt) ? Local : Async;
            }
            else
            {
                return Zilch;
            }
        }

        const char * Connect:: Text(const Mode m) throw()
        {
            switch (m) {
                case Zilch: return "Zilch";
                case Local: return "Local";
                case Async: return "Async";
            }
            return "???";
        }

#define Y_SPADE_CNX_(MODE) unsigned(MODE)
#define Y_SPADE_CNX(A,B) ( (Y_SPADE_CNX_(A)<<8) | (Y_SPADE_CNX_(B)) )
        
        void   Connect:: Authorize(const unsigned level,
                                   const Mode     fwd,
                                   const Mode     rev)
        {
            const unsigned status = Y_SPADE_CNX(fwd,rev);
            switch(status)
            {
                case Y_SPADE_CNX(Zilch,Zilch):
                //case Y_SPADE_CNX(Zilch,Local):
                case Y_SPADE_CNX(Zilch,Async):
                    
                //case Y_SPADE_CNX(Local,Zilch):
                case Y_SPADE_CNX(Local,Local):
                    
                case Y_SPADE_CNX(Async,Zilch):
                case Y_SPADE_CNX(Async,Async):
                    return;
                    
                default:
                    break;
            }
            
            throw exception("Spade::Connect: Unauthorized forward=%s and reverse=%s @level=%u", Text(fwd), Text(rev), level);
        }
        
        
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
                return parops::rank_prev(localSize,localRank);
            }
            
            Coord1D Topology:: Next(Coord1D localSize, Coord1D localRank) throw()
            {
                return parops::rank_next(localSize,localRank);
            }
            
            const Coord1D Coordination<1>::Probes[Levels]= {
                1
            };
            
            const Coord2D  Coordination<2>::Probes[Levels]= {
                Coord2D(1,0),
                Coord2D(0,1),
                
                Coord2D(1,1),
                Coord2D(1,-1)
            };
            
            const Coord3D Coordination<3>::Probes[Levels]= {
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

