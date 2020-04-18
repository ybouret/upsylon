
//! \file

#ifndef Y_IOS_CONVEYORS_INCLUDED
#define Y_IOS_CONVEYORS_INCLUDED 1

#include "y/ios/conveyors.hpp"
#include "y/type/mark.hpp"

namespace upsylon {
    
    namespace ios
    {
        
        //! database of conveyors
        class conveyors : public singleton<conveyors>
        {
        public:
            
            
        private:
            virtual ~conveyors() throw();
            Y_DISABLE_COPY_AND_ASSIGN(conveyors);
            friend class singleton<conveyors>;
        };
        
    }
    
}

#endif

