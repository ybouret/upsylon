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
                static const unsigned Directions = Level1+Level2+Level3; //!< (3^1-1)/2
                static const unsigned Number     = Directions<<1;        //!< 3^1-1
                static const Coord1D  Probes[Directions];
            };
            
            template <> struct Coordination<2>
            {
                static const unsigned Level1     = 2; //!< +X,+Y
                static const unsigned Level2     = 2; //!< +X+Y,+X-Y
                static const unsigned Level3     = 0; //!< none
                static const unsigned Directions = Level1+Level2+Level3; //!< (3^1-1)/2
                static const unsigned Number     = Directions<<1;        //!< 3^1-1
                static const Coord2D Probes[Directions];
            };
            
            template <> struct Coordination<3>
            {
                static const unsigned Level1     = 3;
                static const unsigned Level2     = 6;
                static const unsigned Level3     = 4;
                static const unsigned Directions = Level1+Level2+Level3; //!< (3^1-1)/2
                static const unsigned Number     = Directions<<1;        //!< 3^1-1
                static const Coord3D Probes[Directions];
            };

            //------------------------------------------------------------------
            //
            //! base class for Topology computations
            //
            //------------------------------------------------------------------
            class Topology
            {
            public:
               
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
            static  const unsigned                   Neighbours = Coordination::Number;     //!< alias
            static  const unsigned                   Directions = Coordination::Directions; //!< alias
            typedef typename type_traits<COORD>::mutable_type coord;            //!< alias
            typedef const coord                               const_coord;      //!< alias
            typedef mloop<Coord1D,coord>                      Loop;             //!< loop over ranks if neccessary
         
            
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
            pitch(1)
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
            // members
            //
            //------------------------------------------------------------------
            const_coord  sizes; //!< local sizes
            const_coord  pitch; //!< pitch for ranks algebra
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };
        
        //extern template const Coord1D Topology<Coord1D>::Probes[Directions];

    }
    
}

#endif

