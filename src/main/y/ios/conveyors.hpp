
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
        class vizible;
        
        //! database of conveyors
        class conveyors : public singleton<conveyors>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const conveyor>  convoy;          //!< internal shared conveyor
            Y_SINGLETON_DECL(conveyors);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! search for a previously inserted type
            const conveyor * search(const std::type_info &,
                                    const comms::infrastructure)   const throw();
            
            
            //! query/create conveyor for primary types or serialzable types
            template <typename T>
            const conveyor & query(const comms::infrastructure where)
            {
                Y_LOCK(access);
                
                // prepare typeid
                typedef typename type_traits<T>::mutable_type type;
                static  const std::type_info &tid = typeid(type);
                
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
            
            //! query/create conveyor for tuple of types
            template <template <typename> class TUPLE,
            typename T>
            const conveyor & query(const comms::infrastructure topo)
            {
                Y_LOCK(access);
                // prepare typeid
                typedef typename type_traits<T>::mutable_type             type;
                typedef typename type_traits< TUPLE<type> >::mutable_type tuple_type;
                static  const    std::type_info &tid = typeid(tuple_type);

                // look up
                const conveyor *pc = search(tid,topo);
                if( pc )
                    return *pc;
                else
                {
                    static const bool srz = Y_IS_SUPERSUBCLASS(serializable,type);
                    const convoy      cnv = create_tuple<TUPLE,type>(topo,int2type<srz>());
                    return insert(tid,topo,cnv);
                }
            }

            //! get conveyor by typeid
            const conveyor & get(const std::type_info       &tid,
                                 const comms::infrastructure topo ) const;

            //! get a previously created conveyor
            template <typename T>
            const conveyor & get(const comms::infrastructure topo) const
            {
                // prepare typeid
                typedef typename type_traits<T>::mutable_type type;
                return get( typeid(type), topo);
            }
            
            //! get the internal root node, for graphviz
            const vizible & root() const throw();
            
            void  import(const comms::infrastructure); //!< import integral/tuple types
            void  sort();                              //!< sort by names
            void  display() const;                     //!< display
            
        private:
            virtual ~conveyors() throw();
            explicit conveyors();
            Y_DISABLE_COPY_AND_ASSIGN(conveyors);
            friend class singleton<conveyors>;
            void throw_invalid_infra() const;
            void throw_missing_class(const std::type_info &) const;

            const conveyor & insert(const std::type_info &, const comms::infrastructure, const convoy &);

            template <typename T>
            const conveyor * create(const comms::infrastructure &where,
                                    int2type<false> )
            {
                switch(where)
                {
                    case comms::homogeneous: return new primary_conveyor<T>();
                    case comms::distributed: return new network_conveyor<T>();
                }
                throw_invalid_infra();
                return NULL;
            }
            
            template <typename T>
            const conveyor * create(const comms::infrastructure &,int2type<true>)
            {
                return new derived_conveyor<T>();
            }
            
            template < template <typename> class TUPLE,typename T>
            const conveyor * create_tuple(const comms::infrastructure &where, int2type<false>)
            {
                switch(where)
                {
                    case comms::homogeneous: return new tuple_conveyor<TUPLE,T,primary_conveyor>();
                    case comms::distributed: return new tuple_conveyor<TUPLE,T,network_conveyor>();
                }
                throw_invalid_infra();
                return NULL;
            }
            
            template <template <typename> class TUPLE,
            typename T>
            const conveyor * create_tuple(const comms::infrastructure &,int2type<true>)
            {
                return new tuple_conveyor<TUPLE,T,derived_conveyor>();
            }
            
        };
        
    }
    
}

#endif

