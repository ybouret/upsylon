#include "y/spade/layout/topology.hpp"
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

#define Y_CNX_CASE_TEXT(ID) case ID : return #ID
        
        static const char Unknown[] = "???";
        
        const char * Connect:: ModeText(const Mode m) throw()
        {
            switch(m)
            {
                    Y_CNX_CASE_TEXT(Zilch);
                    Y_CNX_CASE_TEXT(Local);
                    Y_CNX_CASE_TEXT(Async);
            }
            return Unknown;
        }
        
        const char * Connect:: FlagText(const Flag f) throw()
        {
            switch(f)
            {
                    Y_CNX_CASE_TEXT(FreeStanding);
                    Y_CNX_CASE_TEXT(AutoExchange);
                    Y_CNX_CASE_TEXT(AsyncTwoWays);
                    Y_CNX_CASE_TEXT(AsyncForward);
                    Y_CNX_CASE_TEXT(AsyncReverse);
            }
            return Unknown;
        }

        
        bool  Connect::Authorized(const Mode fwd, const Mode rev) throw()
        {
            switch(Y_SPADE_CNX(fwd,rev))
            {
                case FreeStanding:
                case AutoExchange:
                case AsyncTwoWays:
                case AsyncForward:
                case AsyncReverse:
                    return true;
                    
                default:
                    break;
            }
            return false;
        }
        
        void   Connect:: Authorize(const unsigned level,
                                   const Mode     fwd,
                                   const Mode     rev)
        {
            
            if(!Authorized(fwd,rev))
                throw exception("Spade::Connect: Unauthorized forward=%s and reverse=%s @level=%u", ModeText(fwd), ModeText(rev), level);
        }
        
        const char Connect:: TopologyNode[] = "Topology::Node";
        const char Connect:: Layouts[]      = "Layouts";
        
        void Connect:: InvalidFlag(const Flag f,const char *where)
        {
            assert(where);
            throw exception("Spade::Connect: InvalidFlag (%d) in <%s>", int(f), where);
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

