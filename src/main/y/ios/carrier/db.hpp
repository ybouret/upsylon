

//! \file

#ifndef Y_IOS_CARRIER_TUPLE_INCLUDED
#define Y_IOS_CARRIER_TUPLE_INCLUDED 1


#include "y/ios/carrier.hpp"


namespace upsylon
{
    
    namespace ios
    {
        
        class carriers : public singleton<carriers>
        {
        public:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(carriers);
            Y_SINGLETON_DECL_WITH(10,carriers);
            
            virtual ~carriers() throw();
            explicit carriers();
        };
        
        
    }
    
}

#endif
