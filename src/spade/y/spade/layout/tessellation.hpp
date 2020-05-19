//! \file

#ifndef Y_SPADE_TESSELLATION_INCLUDED
#define Y_SPADE_TESSELLATION_INCLUDED 1

#include "y/spade/layout/fragment.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        
        //----------------------------------------------------------------------
        //
        //! tesselation of a full layout
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Tessellation : public Topology<COORD>
        {
        public:
            typedef Layout<COORD>                  LayoutType;
            typedef Fragment<COORD>                FragmentType;
            typedef Topology<COORD>                TopologyType;
            typedef typename TopologyType::Boolean Boolean;
            
            virtual ~Tessellation() throw() {}
            
            explicit Tessellation(const LayoutType & fullLayout,
                               const COORD      & mapping,
                               const COORD      & boundaries,
                               const Coord1D      numGhosts) :
            TopologyType(mapping),
            parts(this->size),
            minItems(0),
            maxItems(0),
            maxComms(0)
            {
                const Boolean pbcs = Coord::ToBool(boundaries);
                for(size_t rank=0;rank<this->size;++rank)
                {
                    const COORD localRanks = this->getLocalRanks(rank);
                    aliasing::_(parts). template build<
                    const LayoutType   &,
                    const COORD        &,
                    const TopologyType &,
                    const Boolean      &,
                    const Coord1D      &>(fullLayout,localRanks,*this,pbcs,numGhosts);
                    const FragmentType &L = parts[rank];
                    aliasing::_(maxItems) = max_of(maxItems,L.inner.items);
                    aliasing::_(maxComms) = max_of(maxComms,L.commScore);
                }
                aliasing::_(minItems) = maxItems;
                for(size_t rank=0;rank<this->size;++rank)
                {
                    aliasing::_(minItems) = min_of(minItems,parts[rank].inner.items);
                }
            }

            inline const FragmentType & operator[](const size_t rank) const throw()
            {
                assert(rank<this->size);
                return parts[rank];
            }

            const slots<FragmentType> parts;
            const size_t              minItems;
            const size_t              maxItems;
            const size_t              maxComms;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tessellation);
        };
    }
    
}

#endif

