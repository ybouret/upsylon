
//! \file

#ifndef Y_IOS_CONVEYORS_INCLUDED
#define Y_IOS_CONVEYORS_INCLUDED 1

#include "y/ios/conveyor/primary.hpp"
#include "y/ios/conveyor/network.hpp"
#include "y/ios/conveyor/derived.hpp"
#include "y/ios/conveyor/tuple.hpp"
#include "y/ptr/arc.hpp"
#include "y/ios/serializable.hpp"
#include "y/type/class-conversion.hpp"

namespace upsylon {
    
    namespace ios
    {
        
        //! database of conveyors
        class conveyors : public singleton<conveyors>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const conveyor>  convoy;          //!< internal shared conveyor
            static  const at_exit::longevity life_time = 0;   //!< for singleton
            
            template <typename T>
            const conveyor & create_for(const comm_environment &where)
            {
                typedef typename type_traits<T>::mutable_type type;
                static const bool            srz = Y_IS_SUPERSUBCLASS(serializable,type);
                static const std::type_info &tid = typeid(type);
                const convoy                 cnv = create<type>(where,int2type<srz>());
                return insert(tid,cnv);
            }
            
        private:
            virtual ~conveyors() throw();
            explicit conveyors();
            Y_DISABLE_COPY_AND_ASSIGN(conveyors);
            friend class singleton<conveyors>;
            
            const conveyor & insert(const std::type_info &, const convoy &);
            const conveyor * search(const std::type_info &) const throw();
            
            template <typename T>
            const conveyor * create(const comm_environment &where,
                                    int2type<false> )
            {
                switch(where)
                {
                    case comm_homogeneous: return new primary_conveyor<T>();
                    case comm_distributed: return new network_conveyor<T>();
                }
            }
            
            template <typename T>
            const conveyor * create(const comm_environment &,
                                    int2type<true> )
            {
                return new derived_conveyor<T>();
            }
            
        };
        
    }
    
}

#endif

