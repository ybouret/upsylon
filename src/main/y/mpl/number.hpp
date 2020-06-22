
//! \file

#ifndef Y_MP_NUMBER_INCLUDED
#define Y_MP_NUMBER_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ptr/counted.hpp"

namespace upsylon {

    
    namespace mpl {
        
        typedef uint64_t word_t;    //!< integral type for drop in replacement
        typedef int64_t  integer_t; //!< signed integral type
        
        //======================================================================
        //
        //! base class to clarify hierarchy
        //
        //======================================================================
        class number_type : public counted_object, public ios::serializable
        {
        public:
            explicit number_type() throw(); //!< setup
            virtual ~number_type() throw(); //!< destructor
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(number_type);
        };
    }
}

#endif

