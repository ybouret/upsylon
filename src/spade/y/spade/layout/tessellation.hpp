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
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>   LayoutType;   //!< alias
            typedef Fragment<COORD> FragmentType; //!< alias
            typedef Topology<COORD> TopologyType; //!< alias

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------

            //! cleanup
            virtual ~Tessellation() throw() {}

            //! initialize
            explicit Tessellation(const LayoutType & fullLayout,
                                  const COORD      & mapping,
                                  const COORD      & boundaries,
                                  const Coord1D      numGhosts) :
            TopologyType(mapping),
            fragments(this->size),
            minItems(0),
            maxItems(0),
            maxComms(0)
            {
                //--------------------------------------------------------------
                // build fragments and collect maxComms and maxItems
                //--------------------------------------------------------------
                for(size_t rank=0;rank<this->size;++rank)
                {
                    const COORD localRanks = this->getLocalRanks(rank);
                    aliasing::_(fragments). template build<
                    const LayoutType   &,
                    const COORD        &,
                    const TopologyType &,
                    const COORD        &,
                    const Coord1D      &>(fullLayout,localRanks,*this,boundaries,numGhosts);
                    const FragmentType &L = fragments[rank];
                    aliasing::_(maxItems) = max_of(maxItems,L.inner.items);
                    aliasing::_(maxComms) = max_of(maxComms,L.commScore);
                }

                //--------------------------------------------------------------
                // collect minItems
                //--------------------------------------------------------------
                aliasing::_(minItems) = maxItems;
                for(size_t rank=0;rank<this->size;++rank)
                {
                    aliasing::_(minItems) = min_of(minItems,fragments[rank].inner.items);
                }
            }

            //! acces fragment in [0..size-1]
            inline const FragmentType & operator[](const size_t rank) const throw()
            {
                assert(rank<this->size);
                return fragments[rank];
            }

            const slots<FragmentType> fragments; //!< fragments of the tessellation
            const size_t              minItems;  //!< min items to process in this tessellation
            const size_t              maxItems;  //!< max items to process in this tesselation
            const size_t              maxComms;  //!< max items for I/O in this tesselation
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tessellation);
        };
    }
    
}

#endif

