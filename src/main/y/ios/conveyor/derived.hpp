//! \file

#ifndef Y_IOS_CONVEYOR_DERIVED_INCLUDED
#define Y_IOS_CONVEYOR_DERIVED_INCLUDED 1

#include "y/ios/conveyor.hpp"

namespace upsylon {
    
    namespace ios
    {
        template <typename T>
        class derived_conveyor : public conveyor
        {
        public:
            inline explicit derived_conveyor() throw() {}
            inline virtual ~derived_conveyor() throw() {}

            inline virtual void copy(void *target, const void *source) const
            {
                assert(target);
                assert(source);
                *static_cast<T*>( target ) = *static_cast<const T *>(source);
            }
            
            inline virtual size_t save( ios::ostream &target, const void *source ) const
            {
                return static_cast<const T*>(source)->serialize(target);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(derived_conveyor);
        };
    }
    
}

#endif



