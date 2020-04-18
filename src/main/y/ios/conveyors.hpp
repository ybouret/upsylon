
//! \file

#ifndef Y_IOS_CONVEYORS_INCLUDED
#define Y_IOS_CONVEYORS_INCLUDED 1

#include "y/ios/conveyors.hpp"
#include "y/type/spec.hpp"

namespace upsylon {
    
    namespace ios
    {
        
        //! database of conveyors
        class conveyors : public singleton<conveyors>
        {
        public:
            
            static const at_exit::longevity life_time = 0;

        private:
            virtual ~conveyors() throw();
            explicit conveyors();
            Y_DISABLE_COPY_AND_ASSIGN(conveyors);
            friend class singleton<conveyors>;
        };
        
    }
    
}

#endif

