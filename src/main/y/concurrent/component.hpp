
//! \file

#ifndef Y_CONCURRENT_COMPONENT_INCLUDED
#define Y_CONCURRENT_COMPONENT_INCLUDED 1

#include "y/concurrent/topology.hpp"

namespace upsylon
{
    
    namespace concurrent
    {

        class component
        {
        public:
            virtual ~component() throw();
            
            mutex             access; //!< shared    access
            topology::pointer topo;   //!< thread(s) topology
            
            
        protected:
            explicit component();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(component);
        };
        
    }
    
}

#endif
