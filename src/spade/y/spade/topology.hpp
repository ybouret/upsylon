//! \file

#ifndef Y_SPADE_TOPOLOGY_INCLUDED
#define Y_SPADE_TOPOLOGY_INCLUDED 1

#include "y/spade/types.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        namespace Kernel
        {
            class Topology
            {
            public:
                virtual ~Topology() throw();
                const size_t cores;
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
            typedef typename type_traits<COORD>::mutable_type coord;         //!< alias
            typedef const coord                               const_coord;   //!< alias
            
            explicit Topology(const_coord mapping) :
            Kernel::Topology( Coord::Product(mapping) ),
            sizes(mapping)
            {
            }
            
            const_coord  sizes;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };
        
    }
    
}

#endif

