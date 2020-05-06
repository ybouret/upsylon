//! \file

#ifndef Y_SPADE_PARTITION_INCLUDED
#define Y_SPADE_PARTITION_INCLUDED 1

#include "y/spade/layout.hpp"
#include "y/sequence/slots.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {
    
    namespace Spade
    {
        namespace Kernel
        {
            //----------------------------------------------------------------------
            //
            //! base class for partition
            //
            //----------------------------------------------------------------------
            class Partition
            {
            public:
                typedef memory::pooled Allocator;              //!< alias
                virtual ~Partition() throw();                  //!< cleanup
                
            protected:
                explicit Partition(const Coord1D   n);         //!< check cores
                explicit Partition(const Partition &) throw(); //!< copy
                
            public:
                const size_t cores;                            //!< cores of this partition
                
            private:
                Y_DISABLE_ASSIGN(Partition);
            };
        }
        
        //----------------------------------------------------------------------
        //
        //! partition of a full layout
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Partition : public Kernel::Partition, public Layout<COORD>, public slots<Layout<COORD>,Kernel::Partition::Allocator>
        {
        public:
            typedef Layout<COORD>                    LayoutType;  //!< alias
            typedef Kernel::Partition::Allocator     Allocator;   //!< alias
            typedef slots<LayoutType,Allocator>      Slots;       //!< alias
            typedef typename LayoutType::coord       coord;       //!< alias
            typedef typename LayoutType::const_coord const_coord; //!< alias
            
            //! setup from coordinates
            inline explicit Partition(const_coord lo,
                                      const_coord up,
                                      const_coord sizes) :
            Kernel::Partition( Coord::Product(sizes) ),
            LayoutType(lo,up),
            Slots(cores),
            maxItems( setup(sizes) )
            {
            }
            
            //! setup from a full layout
            inline explicit Partition(const LayoutType &full,
                                      const_coord       sizes) :
            Kernel::Partition( Coord::Product(sizes) ),
            LayoutType(full),
            Slots(cores),
            maxItems(setup(sizes))
            {
            }
            
            //! cleanup
            inline virtual ~Partition() throw()
            {
                
            }
            
            const size_t maxItems; //!< max items in sub-layouts
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Partition);
            
            inline size_t setup(const_coord &sizes)
            {
                // prepare loop over 0..sizes-1
                typename LayoutType::Loop loop( Coord::Zero<coord>(), sizes - Coord::Ones<coord>() );
                const_coord              &ranks = loop.value;
                size_t                    itmax = 0;
                
                for(loop.boot();loop.good();loop.next())
                {
                    const LayoutType sub = this->split(sizes,ranks);
                    this->push(sub);
                    itmax = max_of(itmax,sub.items);
                }
                assert( this->size() == cores);
                return itmax;
            }
        };
    }
    
}

#endif

