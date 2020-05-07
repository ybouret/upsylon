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
            
            template <> struct Coordination<1>
            {
                static const unsigned Level1  = 1; //!< +X
                static const unsigned Level2  = 0; //!< none
                static const unsigned Level3  = 0; //!< none
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord1D  Probes[Levels];                   //!< unique probes
            };
            
            template <> struct Coordination<2>
            {
                static const unsigned Level1  = 2; //!< +X,+Y
                static const unsigned Level2  = 2; //!< +X+Y,+X-Y
                static const unsigned Level3  = 0; //!< none
                static const unsigned Levels  = Level1+Level2+Level3;   //!< (3^DIM-1)/2
                static const unsigned Number  = Levels<<1;              //!< neighbours
                static const Coord2D  Probes[Levels];                   //!< unique probes
            };
            
            template <> struct Coordination<3>
            {
                static const unsigned Level1  = 3; //!< +X,+Y
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
                //! direction for each way
                enum Direction
                {
                    Forward, //!< forward in the direction
                    Reverse  //!< reverse in the direction
                };
                
                virtual ~Topology() throw();        //!< cleanup
                const size_t size;                  //!< number of cores
                
                //! previous local rank w.r.t local size
                static Coord1D Prev(Coord1D localSize, Coord1D localRank) throw();
                
                //! next local rank w.r.t. local size
                static Coord1D Next(Coord1D localSize, Coord1D localRank) throw();

            protected:
                explicit Topology(const size_t nc); //!< set size, with checking
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Topology);
            };
        };
        
        //----------------------------------------------------------------------
        //
        //! Topology computation
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
                    const Coord1D p = Coord::Of(probe,dim);
                    const Coord1D localSize = Coord::Of(sizes,dim);
                    const Coord1D localRank = Coord::Of(ranks,dim);
                    switch(p)
                    {
                        case  1: Coord::Of(where,dim) = Next(localSize,localRank); break;
                        case -1: Coord::Of(where,dim) = Prev(localSize,localRank); break;
                        default: assert(0==p); break;
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
            const Boolean parallel; //!< dimension wise parallel
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };
        

        template <typename COORD>
        class Hub
        {
        public:
            typedef Topology<COORD>                     Topo;
            static const unsigned Dimensions = Topo::Dimensions;
            typedef typename Topo::coord                coord;
            typedef typename Topo::const_coord          const_coord;
            typedef typename Topo::Boolean              Boolean;
            
            const size_t  rank;
            const_coord   ranks;
            const Boolean bulk;
            
            explicit Hub(const size_t   r,
                         const Topo    &topo,
                         const Boolean &pbc) :
            rank(r),
            ranks( topo.getLocalRanks(rank) ),
            bulk( Coord::False<Boolean>() )
            {
                
            }
            
        private:
            
        };
        
    }
    
}

#endif

