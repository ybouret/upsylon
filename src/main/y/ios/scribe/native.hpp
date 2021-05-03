
//! \file

#ifndef Y_IOS_SCRIBE_NATIVE_INCLUDED
#define Y_IOS_SCRIBE_NATIVE_INCLUDED 1

#include "y/ios/scribe/in1d.hpp"

namespace upsylon
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! class to write native types
        //
        //______________________________________________________________________
        class native_scribe : public scribe1D
        {
        public:
            virtual ~native_scribe() throw(); //!< cleanup
            string fmt;                       //!< user defined format
            
        protected:
            //! setup with type and default format
            explicit native_scribe(const std::type_info &tid,
                                   const size_t          tsz,
                                   const char           *how);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(native_scribe);
        };
        
    }
    
}

#endif
