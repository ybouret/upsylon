//! \file

#ifndef Y_SPADE_TESSELLATION_INCLUDED
#define Y_SPADE_TESSELLATION_INCLUDED 1

#include "y/spade/layout/fragment.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon {
    
    namespace Spade
    {
        namespace Kernel
        {
            //! common stuff for tessellations
            class Tessellation
            {
            public:
                const unsigned dimensions;        //!< layout dimensions
                virtual ~Tessellation() throw();  //!< cleanup

            protected:
                explicit Tessellation(const unsigned) throw();  //!< setup
                static void throwNoMapping(const unsigned dim); //!< no possible mapping excetpion

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Tessellation);
            };
        }
        
        //----------------------------------------------------------------------
        //
        //! tesselation of a full layout
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Tessellation : public Object, public Kernel::Tessellation, public Dispatch<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef arc_ptr<const Tessellation> Handle;       //!< alias
            typedef Layout<COORD>               LayoutType;   //!< alias
            typedef Fragment<COORD>             FragmentType; //!< alias
            typedef Dispatch<COORD>             DispatchType; //!< alias
            static const unsigned               Dimensions = LayoutType::Dimensions; //!< alias

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
            Kernel::Tessellation(Dimensions),
            DispatchType(mapping),
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
                    const DispatchType &,
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

            //! optimized mapping
            static inline
            COORD Find(const Layout<COORD> &fullLayout,
                       const size_t         cores,
                       const COORD         &boundaries,
                       const Coord1D        numGhosts)
            {
                //--------------------------------------------------------------
                // local types
                //--------------------------------------------------------------
                typedef vector<COORD,memory::pooled>  Mappings;
                typedef vector<Handle,memory::pooled> Tess;

                //--------------------------------------------------------------
                // find all mappings
                //--------------------------------------------------------------
                std::cerr << "\t\t\tTessellations Of " << fullLayout << std::endl;
                Mappings     mappings;
                const size_t count = fullLayout.findMappings(mappings,cores);
                if(count<=0) throwNoMapping(Dimensions);

                //--------------------------------------------------------------
                // compute associated tessellations
                //--------------------------------------------------------------
                Tess         tess(count,as_capacity);
                for(size_t i=1;i<=count;++i)
                {
                    const Handle t = new Tessellation(fullLayout,mappings[i],boundaries,numGhosts);
                    tess.push_back_(t);
                }

                //--------------------------------------------------------------
                // sort by maxItems and keep smallest maxItems
                //--------------------------------------------------------------
                hsort(tess,byIncreasingMaxItems);
                while( tess.size() > 1 && tess.back()->maxItems>tess.front()->maxItems)
                {
                    tess.pop_back();
                }
                
                //--------------------------------------------------------------
                // sort by maxComms and keep smallest maxComms
                //--------------------------------------------------------------
                hsort(tess,byIncreasingMaxComms);
                while( tess.size() > 1 && tess.back()->maxComms>tess.front()->maxComms)
                {
                    tess.pop_back();
                }

                //--------------------------------------------------------------
                // sort by increasing lexicographic sizes
                //--------------------------------------------------------------
                hsort(tess,byIncreasingMappings);
                for(size_t i=1;i<=tess.size();++i)
                {
                    std::cerr << "\t\t\t|_tess[" << i << "]: " << tess[i]->sizes << " | maxItems: " << tess[i]->maxItems << " | maxComms: " << tess[i]->maxComms << std::endl;
                }
                return Coord::Zero<COORD>();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tessellation);
            static inline int byIncreasingMaxItems(const Handle &lhs, const Handle &rhs) throw()
            {
                return comparison::increasing(lhs->maxItems,rhs->maxItems);
            }

            static inline int byIncreasingMaxComms(const Handle &lhs, const Handle &rhs) throw()
            {
                return comparison::increasing(lhs->maxComms,rhs->maxComms);
            }

            static inline int byIncreasingMappings(const Handle &lhs, const Handle &rhs) throw()
            {
                return Coord::Increasing(lhs->sizes, rhs->sizes);
            }

        };
    }
    
}

#endif

