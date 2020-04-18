//! \file

#ifndef Y_IOS_CONVEYOR_TUPLE_INCLUDED
#define Y_IOS_CONVEYOR_TUPLE_INCLUDED 1

#include "y/ios/conveyor/limited.hpp"

namespace upsylon {
    
    namespace ios
    {
        
        template <
        template <typename> class TUPLE,
        typename                  T,
        template <typename> class CONVEYOR
        >
        class tuple_conveyor : public limited_conveyor<T,CONVEYOR>
        {
        public:
            inline virtual ~tuple_conveyor() throw()
            {
            }
            
            inline explicit tuple_conveyor() throw() :
            limited_conveyor<T,CONVEYOR>( sizeof( TUPLE<T> )/sizeof(T) )
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(tuple_conveyor);
        };
    }
    
}

#endif

