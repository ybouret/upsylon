
//! \file

#ifndef Y_CONCURRENT_COMPONENT_INCLUDED
#define Y_CONCURRENT_COMPONENT_INCLUDED 1

#include "y/concurrent/topology.hpp"

namespace upsylon
{
    
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! a component for threads
        //
        //______________________________________________________________________
        class component
        {
        public:
            virtual ~component() throw(); //!< cleanup
            
            mutex             access;     //!< shared    access
            topology::pointer topo;       //!< thread(s) topology
            
            
        protected:
            explicit component();         //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(component);
        };
        
    }
    
}

#endif
