
//! \file

#ifndef Y_IOS_CARRIER_TUPLE_INCLUDED
#define Y_IOS_CARRIER_TUPLE_INCLUDED 1

#include "y/ios/carrier/limited.hpp"

namespace upsylon {
    
    namespace ios
    {
        
        //----------------------------------------------------------------------
        //
        //! conveyor of tuples of same types
        //
        //----------------------------------------------------------------------
        template <
        template <typename> class TUPLE,
        typename                  T,
        template <typename> class CONVEYOR
        >
        class tuple_carrier : public limited_carrier<sizeof( TUPLE<T> )/sizeof(T),T,CONVEYOR>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! cleanup
            inline virtual ~tuple_carrier() throw() {}
            
            //! setup
            inline explicit tuple_carrier() throw() :
            limited_carrier<sizeof( TUPLE<T> )/sizeof(T),T,CONVEYOR>(typeid( typename type_traits< TUPLE<T> >::mutable_type ) )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tuple_carrier);
        };
    }
    
}

#endif

