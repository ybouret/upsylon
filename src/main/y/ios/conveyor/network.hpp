
//! \file

#ifndef Y_IOS_CONVEYOR_NETWORK_INCLUDED
#define Y_IOS_CONVEYOR_NETWORK_INCLUDED 1

#include "y/ios/conveyor.hpp"
#include "y/os/endian.hpp"
#include "y/type/spec.hpp"
#include "y/type/args.hpp"

namespace upsylon {
    
    namespace ios
    {
        //! conveyor for primary types in heterogeneous environment
        template <typename T>
        class network_conveyor : public conveyor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                                         //!< aliases
            typedef typename unsigned_int<sizeof(type)>::type word_type; //!< for big endian
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            inline explicit network_conveyor() throw() : conveyor(comm_constant_size) {}
           
            //!cleanup
            inline virtual ~network_conveyor() throw() {}
            
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
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
            
            inline virtual size_t load(void *target, ios::istream &source) const
            {
                assert(target);
                word_type temp = 0;
                if( source.try_query(&temp,sizeof(T)) != sizeof(T) ) missing_bytes( type_name_of<T>() );
                *static_cast<word_type *>(target) = swap_be(temp);
                return sizeof(T);
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(network_conveyor);
        };
    }
    
}

#endif



