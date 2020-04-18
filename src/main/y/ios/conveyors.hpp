
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
            const conveyor & query(const comms::medium &where)
            {
                Y_LOCK(access);
                // prepare typeid
                typedef typename type_traits<T>::mutable_type type;
                static const std::type_info &tid = typeid(type);
                
                // look up
                const conveyor *pc = search(tid,where);
                if( pc )
                {
                    return *pc;
                }
                else
                {
                    static const bool srz = Y_IS_SUPERSUBCLASS(serializable,type);
                    const convoy      cnv = create<type>(where,int2type<srz>());
                    return insert(tid,where,cnv);
                }
            }
            
            
            
        private:
            virtual ~conveyors() throw();
            explicit conveyors();
            Y_DISABLE_COPY_AND_ASSIGN(conveyors);
            friend class singleton<conveyors>;
            void throw_invalid_environment() const;


            const conveyor & insert(const std::type_info   &,
                                    const comms::medium,
                                    const convoy           &);
            
            const conveyor * search(const std::type_info   &,
                                    const comms::medium) const throw();

            template <typename T>
            const conveyor * create(const comms::medium &where,
                                    int2type<false> )
            {
                switch(where)
                {
                    case comms::homogeneous: return new primary_conveyor<T>();
                    case comms::distributed: return new network_conveyor<T>();
                }
                throw_invalid_environment();
                return NULL;
            }
            
            template <typename T>
            const conveyor * create(const comms::medium &,
                                    int2type<true> )
            {
                return new derived_conveyor<T>();
            }
            
        };
        
    }
    
}

#endif

