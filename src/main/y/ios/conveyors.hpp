
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
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! conveyor for primary types or serialzable types
            template <typename T>
            const conveyor & query(const comms::topology &where)
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
            
            //! conveyor for tuple of types
            template <template <typename> class TUPLE,
            typename T>
            const conveyor & query(const comms::topology &where)
            {
                Y_LOCK(access);
                // prepare typeid
                typedef typename type_traits<T>::mutable_type             type;
                typedef typename type_traits< TUPLE<type> >::mutable_type tuple_type;
                static  const    std::type_info &tid = typeid(tuple_type);
                // look up
                const conveyor *pc = search(tid,where);
                if( pc )
                {
                    return *pc;
                }
                else
                {
                    static const bool srz = Y_IS_SUPERSUBCLASS(serializable,type);
                    const convoy      cnv = create_tuple<TUPLE,type>(where,int2type<srz>());
                    return insert(tid,where,cnv);
                }
                
            }
            
            void graphViz(const char   *) const;
            void graphViz(const string &) const;

            
        private:
            virtual ~conveyors() throw();
            explicit conveyors();
            Y_DISABLE_COPY_AND_ASSIGN(conveyors);
            friend class singleton<conveyors>;
            void throw_invalid_topology() const;


            const conveyor & insert(const std::type_info   &,
                                    const comms::topology,
                                    const convoy           &);
            
            const conveyor * search(const std::type_info   &,
                                    const comms::topology) const throw();

            template <typename T>
            const conveyor * create(const comms::topology &where,
                                    int2type<false> )
            {
                switch(where)
                {
                    case comms::homogeneous: return new primary_conveyor<T>();
                    case comms::distributed: return new network_conveyor<T>();
                }
                throw_invalid_topology();
                return NULL;
            }
            
            template <typename T>
            const conveyor * create(const comms::topology &,int2type<true>)
            {
                return new derived_conveyor<T>();
            }
            
            template < template <typename> class TUPLE,typename T>
            const conveyor * create_tuple(const comms::topology &where, int2type<false>)
            {
                switch(where)
                {
                    case comms::homogeneous: return new tuple_conveyor<TUPLE,T,primary_conveyor>();
                    case comms::distributed: return new tuple_conveyor<TUPLE,T,network_conveyor>();
                }
                throw_invalid_topology();
                return NULL;
            }
            
            template <template <typename> class TUPLE,
            typename T>
            const conveyor * create_tuple(const comms::topology &,int2type<true>)
            {
                return new tuple_conveyor<TUPLE,T,derived_conveyor>();
            }
            
        };
        
    }
    
}

#endif

