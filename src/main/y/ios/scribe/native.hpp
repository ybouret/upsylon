
//! \file

#ifndef Y_IOS_SCRIBE_NATIVE_INCLUDED
#define Y_IOS_SCRIBE_NATIVE_INCLUDED 1

#include "y/ios/scribe.hpp"

namespace upsylon
{
    namespace ios
    {
        
        //! class to write native types
        class native_scribe : public scribe
        {
        public:
            virtual ~native_scribe() throw();
            
            string fmt;
            
        protected:
            explicit native_scribe(const std::type_info &tid,
                                   const char           *how);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(native_scribe);
        };
        
    }
    
}

#endif
