//! \file

#ifndef Y_SPADE_TOPOLOGY_INCLUDED
#define Y_SPADE_TOPOLOGY_INCLUDED 1

#include "y/spade/types.hpp"
#include "y/counting/mloop.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        namespace Kernel
        {
            //! Number of neighbours=3^DIM-1
            template <size_t DIM> struct Coordination;
            
            //! Coordination in 1D
            template <> struct Coordination<1>
            {
                static const unsigned Level1  = 1; //!< +X
                static const unsigned Level2  = 0; //!< none
                static const unsigned Level3  = 0; //!< none
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord1D  Probes[Levels];                   //!< unique probes
            };
            
            //! Coordination in 2D
            template <> struct Coordination<2>
            {
                static const unsigned Level1  = 2; //!< +X,+Y
                static const unsigned Level2  = 2; //!< +X+Y,+X-Y
                static const unsigned Level3  = 0; //!< none
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord2D  Probes[Levels];                   //!< unique probes
            };
            
            //! Coordination in 3D
            template <> struct Coordination<3>
            {
                static const unsigned Level1  = 3; //!< +X,+Y,+Z
                static const unsigned Level2  = 6; //!< ...
                static const unsigned Level3  = 4; //!< ...
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord3D  Probes[Levels];                   //!< unique probes
            };
            
            //------------------------------------------------------------------
            //
            //! base class for Topology computations
            //
            //------------------------------------------------------------------
            class Topology
            {
            public:
                //! direction for each Level
                enum Direction
                {
                    Forward, //!< forward at a given level
                    Reverse  //!< reverse at a given level
                };
                
                virtual ~Topology() throw();        //!< cleanup
                const size_t size;                  //!< number of cores
                
                //! reverse (previous) local rank w.r.t local size
                static Coord1D GetReverse(Coord1D localSize, Coord1D localRank) throw();
                
                //! forward(next) local rank w.r.t. local size
                static Coord1D GetForward(Coord1D localSize, Coord1D localRank) throw();
                
            protected:
                explicit Topology(const size_t nc); //!< set size, with checking
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Topology);
            };
        };
        
        //----------------------------------------------------------------------
        //
        //! Topology computation for ranks w.r.t sizes
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Topology : public Kernel::Topology
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static  const unsigned                   Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            typedef Kernel::Coordination<Dimensions> Coordination;                               //!< alias
            static  const unsigned                   Levels = Coordination::Levels; //!< alias
            typedef typename type_traits<COORD>::mutable_type coord;                //!< alias
            typedef const coord                               const_coord;          //!< alias
            typedef typename Coord::Get<COORD>::Boolean       Boolean;              //!< alias
            typedef mloop<Coord1D,coord>                      Loop;                 //!< loop over ranks if neccessary
            
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Topology() throw()
            {
                _bzset(sizes);
                _bzset(pitch);
            }
            
            
            //! setup from a mapping
            explicit Topology(const_coord mapping) :
            Kernel::Topology( Coord::Product(mapping) ),
            sizes(mapping),
            pitch(1),
            maxRanks( sizes - Coord::Ones<coord>() ),
            parallel( Coord::ToParallel(sizes) )
            {
                Coord1D       *p = (Coord1D *)&pitch;
                const Coord1D *s = (const Coord1D *)&sizes;
                for(unsigned dim=1;dim<Dimensions;++dim)
                {
                    p[dim] = p[dim-1] * s[dim-1];
                }
            }
            
            //------------------------------------------------------------------
            //
            // ranks conversion
            //
            //------------------------------------------------------------------
            
            //! rank = ranks.x + sizes.x * ranks.y + sizes.x * sizes.y * ranks.z;
            inline coord getLocalRanks(const size_t rank) const throw()
            {
                assert(rank<size);
                coord   q(0);
                Coord1D rem = Coord1D(rank);
                for(unsigned dim=Dimensions-1;dim>0;--dim)
                {
                    const Coord1D den = Coord::Of(pitch,dim);
                    const Coord1D qot = rem / den;
                    Coord::Of(q,dim) = qot;
                    rem -= qot * den;
                }
                Coord::Of(q,0) = rem;
                assert( Coord::LT(q,sizes) );
                return q;
            }
            
            //! rank = ranks.x + sizes.x * ranks.y + sizes.x * sizes.y * ranks.z;
            inline size_t getGlobalRank(const_coord ranks) const throw()
            {
                assert( Coord::LT(ranks,sizes) );
                assert( Coord::GEQ(ranks,Coord::Zero<coord>()) );
                size_t rank = Coord::Of(ranks,0);
                for(unsigned dim=1;dim<Dimensions;++dim)
                {
                    rank += Coord::Of(pitch,dim) * Coord::Of(ranks,dim);
                }
                return rank;
            }
            
            //------------------------------------------------------------------
            //
            // findind ranks
            //
            //------------------------------------------------------------------
            
            //! rank for a level and a direction
            inline coord getNeighbourRanks(const_coord     ranks,
                                           const unsigned  level,
                                           const Direction direction) const throw()
            {
                assert( Coord::LT(ranks,sizes) );
                assert( Coord::GEQ(ranks,Coord::Zero<coord>()) );
                assert(level<Levels);
                
                // get probe, with sign according to direction
                coord probe = Coordination::Probes[level];
                switch(direction)
                {
                    case Reverse: probe=-probe; break;
                    case Forward: break;
                }
                
                // build neighbour ranks
                coord  where = ranks;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D localScan = Coord::Of(probe,dim);
                    const Coord1D localSize = Coord::Of(sizes,dim);
                    const Coord1D localRank = Coord::Of(ranks,dim);
                    switch(localScan)
                    {
                        case  1: Coord::Of(where,dim) = GetForward(localSize,localRank); break;
                        case -1: Coord::Of(where,dim) = GetReverse(localSize,localRank); break;
                        default: assert(0==localScan); break;
                    }
                }
                return where;
            }
            
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const_coord   sizes;    //!< local sizes
            const_coord   pitch;    //!< pitch for ranks algebra
            const_coord   maxRanks; //!< sizes-1
            const Boolean parallel; //!< dimension wise parallel flag
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };
        
        
    }
    
}

#include "y/sequence/slots.hpp"
namespace upsylon {
    
    namespace Spade
    {
        template <typename COORD>
        struct Connectivity
        {
            typedef Topology<COORD>            Topo;
            typedef typename Topo::coord       coord;
            typedef typename Topo::const_coord const_coord;
            
            class Hub
            {
            public:
                inline explicit Hub(const_coord  localRanks,
                             const Topo  &topo) throw():
                ranks( localRanks ),
                rank( topo.getGlobalRank(ranks) )
                {
                }
                
                inline virtual ~Hub() throw()
                {
                }
                
                inline Hub(const Hub &hub) throw() :
                ranks(hub.ranks),
                rank(hub.rank)
                {
                }
                
                
                const_coord  ranks;
                const size_t rank;
                
            private:
                Y_DISABLE_ASSIGN(Hub);
            };
            
            class Link : public Hub
            {
            public:
                
                inline explicit Link(const_coord  localRanks,
                                     const Topo  &topo) : Hub(localRanks,topo)
                {
                }
                
                inline virtual ~Link() throw()
                {
                }
                
                inline Link( const Link &link) throw() : Hub(link)
                {
                }
                
                
            private:
                Y_DISABLE_ASSIGN(Link);
            };
            
            class Links
            {
            public:
                const Link forward;
                const Link reverse;
                
                explicit Links(const_coord fwd,
                               const_coord rev,
                               const Topo &topo)  throw() :
                forward(fwd,topo), reverse(rev,topo)
                {
                }
                
                virtual ~Links() throw()
                {
                    
                }
                
                Links(const Links &other) throw() :
                forward(other.forward), reverse(other.reverse)
                {
                }
                
                
            private:
                Y_DISABLE_ASSIGN(Links);
            };
            
            
            
            class Node : public Hub, public slots<Links>
            {
            public:
                inline explicit Node(const_coord  localRanks,
                                     const Topo  &topo) :
                Hub(localRanks,topo),
                slots<Link>( Topo::Levels )
                {
                    for(unsigned level=0;level<Topo::Levels;++level)
                    {
                        const_coord fwd = getNeighbourRanks(this->ranks,level,Topo::Forward);
                        const_coord rev = getNeighbourRanks(this->ranks,level,Topo::Reverse);
                        Links       links(fwd,rev,topo);
                        this->push(links);
                    }
                }
                
                inline virtual ~Node() throw()
                {
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };
            
            
        };
        
        
        
    }
    
}

#endif

