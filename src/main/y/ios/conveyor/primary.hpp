
//! \file

#ifndef Y_IOS_CONVEYOR_PRIMARY_INCLUDED
#define Y_IOS_CONVEYOR_PRIMARY_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace ios
    {
        //! conveyor for primary types in homogenous environment
        template <typename T>
        class primary_conveyor : public conveyor
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            
            //! setup
            inline explicit primary_conveyor() throw() :
            conveyor(comms::computed_block_size,
                     comms::homogeneous,
                     typeid(typename type_traits<T>::mutable_type),
                     sizeof(T))
            {}
            
            //! cleanup
            inline virtual ~primary_conveyor() throw() {}
            
            //! memory copy
            inline virtual size_t copy(void *target, const void *source) const
            {
                assert(target);
                assert(source);
                memcpy(target,source,sizeof(T));
                return sizeof(T);
            }
           
            //! write bytes
            inline virtual size_t save(ios::ostream &target, const void *source) const
            {
                assert(source);
                target.output(source,sizeof(T));
                return sizeof(T);
            }
            
            //! read bytes
            inline virtual size_t load(void *target, ios::istream &source) const
            {
                assert(target);
                const size_t read = source.try_query(target, sizeof(T) );
                if(read!=sizeof(T)) missing_bytes();
                return sizeof(T);
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(primary_conveyor);
        };
    }
    
}

#endif

    
    
