
//! \file
#ifndef Y_CHEMICAL_REPLICA_INCLUDED
#define Y_CHEMICAL_REPLICA_INCLUDED 1

#include "y/chemical/equilibria.hpp"


namespace upsylon
{
    namespace Chemical
    {
        
        class Replica : public Object
        {
        public:
            typedef arc_ptr<const Replica>    Pointer;
            typedef vector<Pointer,Allocator> Array;
            
            virtual ~Replica() throw();
            explicit Replica(const Species &, const Accessible &) throw();
            
            const Species    &species;
            const Accessible &v;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Replica);
        };
        
    }
    
}

#endif
