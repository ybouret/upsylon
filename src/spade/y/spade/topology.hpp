//! \file

#ifndef Y_SPADE_TOPOLOGY_INCLUDED
#define Y_SPADE_TOPOLOGY_INCLUDED 1

#include "y/spade/types.hpp"
#include "y/counting/mloop.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        namespace Kernel
        {
            class Topology
            {
            public:
                virtual ~Topology() throw();
                const size_t size;
                
            protected:
                explicit Topology(const size_t nc);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Topology);
            };
        };
        
        template <typename COORD>
        class Topology : public Kernel::Topology
        {
        public:
            static  const unsigned  Dimensions = Coord::Get<COORD>::Dimensions;
            typedef typename type_traits<COORD>::mutable_type coord;         //!< alias
            typedef const coord                               const_coord;   //!< alias
            typedef mloop<Coord1D,coord>                      Loop;          //!< loop over ranks if neccessary
         
            explicit Topology(const_coord mapping) :
            Kernel::Topology( Coord::Product(mapping) ),
            sizes(mapping),
            pitch(Coord::Ones<coord>())
            {
                Coord1D       *p = (Coord1D *)&pitch;
                const Coord1D *s = (const Coord1D *)&sizes;
                for(unsigned dim=1;dim<Dimensions;++dim)
                {
                    p[dim] = p[dim-1] * s[dim-1];
                }
            }
            
            const_coord  sizes;
            const_coord  pitch;
            
            //! rank = rank.x + sizes.x * rank.y + sizes.x * sizes.y * rank.z;
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
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };
        
    }
    
}

#endif

