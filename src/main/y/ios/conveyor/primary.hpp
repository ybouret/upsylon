
//! \file

#ifndef Y_IOS_CONVEYOR_PRIMARY_INCLUDED
#define Y_IOS_CONVEYOR_PRIMARY_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace ios
    {
        template <typename T>
        class primary_conveyor : public conveyor
        {
        public:
            Y_DECL_ARGS(T,type);
            
            inline explicit primary_conveyor() throw() : conveyor() {}
            inline virtual ~primary_conveyor() throw() {}
            
            inline virtual void copy(void *target, const void *source) const
            {
                assert(target);
                assert(source);
                memcpy(target,source,sizeof(T));
            }
           
            inline virtual size_t save(ios::ostream &target, const void *source) const
            {
                assert(source);
                target.output(source,sizeof(T));
                return sizeof(T);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(primary_conveyor);
        };
    }
    
}

#endif

    
    
