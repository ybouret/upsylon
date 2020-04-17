
//! \file

#ifndef Y_IOS_CONVEYOR_NETWORK_INCLUDED
#define Y_IOS_CONVEYOR_NETWORK_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/os/endian.hpp"

namespace upsylon {
    
    namespace ios
    {
        template <typename T>
        class network_conveyor : public conveyor
        {
        public:
            typedef typename unsigned_int<sizeof(T)>::type word_type;
            
            inline explicit network_conveyor() throw() {}

            inline virtual ~network_conveyor() throw() {}
            
            inline virtual void copy(void *target, const void *source) const
            {
                assert(target);
                assert(source);
                memcpy(target,source,sizeof(T));
            }
            
            inline virtual size_t save(ios::ostream &target, const void *source) const
            {
                assert(source);
                const word_type temp = swap_be( *static_cast<const word_type *>(source) );
                target.output(&temp,sizeof(T));
                return sizeof(T);
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(network_conveyor);
        };
    }
    
}

#endif



