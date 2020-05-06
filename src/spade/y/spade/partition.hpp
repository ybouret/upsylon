//! \file

#ifndef Y_SPADE_PARTITION_INCLUDED
#define Y_SPADE_PARTITION_INCLUDED 1

#include "y/spade/layout.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {
    
    namespace Spade
    {
        namespace Kernel
        {
            class Partition
            {
            public:
                virtual ~Partition() throw();
            protected:
                explicit Partition(const Coord1D n);
                explicit Partition(const Partition &);
            public:
                const size_t cores;
                
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
        class Partition : public Kernel::Partition, public Layout<COORD>, public slots< Layout<COORD> >
        {
        public:
            typedef Layout<COORD>                    LayoutType;
            typedef slots<LayoutType>                Slots;
            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;
            
            inline explicit Partition(const_coord lo,
                                      const_coord up,
                                      const_coord sizes) :
            Kernel::Partition( Coord::Product(sizes) ),
            LayoutType(lo,up),
            Slots(cores)
            {
            }
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Partition);
        };
    }
    
}

#endif

