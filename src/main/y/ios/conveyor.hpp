//! \file

#ifndef Y_IOS_CONVEYOR_INCLUDED
#define Y_IOS_CONVEYOR_INCLUDED 1

#include "y/ios/ostream.hpp"

namespace upsylon {
 
    namespace ios
    {
        
        //! abstract function to convey data
        class conveyor
        {
        public:
            virtual ~conveyor() throw();
            
            //! direct and local copy
            virtual void copy(void *target, const void *source) const = 0;
            
            //! save for later
            virtual size_t save(ios::ostream &target, const void *source) const = 0;
            
        protected:
            explicit conveyor() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(conveyor);
        };
        
    }
    
}

#endif

