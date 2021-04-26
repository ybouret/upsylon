

//! \file

#ifndef Y_IOS_CARRIER_DB_INCLUDED
#define Y_IOS_CARRIER_DB_INCLUDED 1


#include "y/ios/carrier/primary.hpp"
#include "y/ios/carrier/network.hpp"
#include "y/ios/carrier/derived.hpp"
#include "y/ios/carrier/tuple.hpp"

#include "y/hashing/type-info.hpp"
#include "y/associative/hash/set.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! databases of type carriers
        //
        //______________________________________________________________________
        class carriers : public singleton<carriers>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef hashing::type_info_hasher<hashing::fnv>          hasher;   //!< direct hasher
            typedef hash_set<std::type_info,carrier::pointer,hasher> db_type;  //!< alias
            typedef db_type::const_iterator                          iterator; //!< alias
            
            
            //__________________________________________________________________
            //
            // record plain type
            //__________________________________________________________________
            //! record a carrier for a plain type
            template <typename T, template <class> class CARRIER>
            static inline void record(db_type &db)
            {
                const carrier::pointer ptr = new CARRIER<T>();
                (void) db.insert(ptr);
            }
            
            //! record two carriers for a plain type
            template <typename T>
            inline void record()
            {
                record<T,primary_carrier>( aliasing::_(homogeneous) );
                record<T,network_carrier>( aliasing::_(distributed) );
            }
            
            //__________________________________________________________________
            //
            // record tuples
            //__________________________________________________________________
            
            //! record a carrier for a tuple of plain types
            template <
            template <class> class TUPLE,
            typename               T,
            template <class> class CARRIER>
            static inline void record_tuple(db_type &db)
            {
                const carrier::pointer ptr = new tuple_carrier<TUPLE,T,CARRIER>();
                (void) db.insert(ptr);
            }
            
            //! record two carriers for a tuple of plain types
            template <
            template <class> class TUPLE,
            typename               T
            >
            inline void record_tuple()
            {
                record_tuple<TUPLE,T,primary_carrier>( aliasing::_(homogeneous) );
                record_tuple<TUPLE,T,network_carrier>( aliasing::_(distributed) );
            }
            
            //__________________________________________________________________
            //
            // record serializable types
            //__________________________________________________________________
            //! record a carrier for a serializable type
            template <typename T>
            inline void use()
            {
                const carrier::pointer ptr = new derived_carrier<T>();
                (void) aliasing::_(distributed).insert(ptr);
            }
            
            //__________________________________________________________________
            //
            // access method
            //__________________________________________________________________
            //! search, safe
            const carrier * search(const std::type_info       &tid,
                                   const comms::infrastructure infra) const throw();
            
            //! search, safe, wrapper
            template <typename T> inline
            const carrier * search(const comms::infrastructure infra) const throw()
            {
                return search(typeid(T),infra);
            }
            
            //! get, unsafe
            const carrier & query(const std::type_info       &tid,
                                  const comms::infrastructure infra,
                                  const char                 *where) const;
            
            //! get, unsafe, weapper
            template <typename T> inline
            const carrier & query(const comms::infrastructure infra,
                                  const char                 *where) const
            {
                return query(typeid(T),infra,where);
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const db_type homogeneous; //!< database of homogeneous carriers
            const db_type distributed; //!< database of distributed carriers
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(carriers);
            Y_SINGLETON_DECL_WITH(10,carriers);
            virtual ~carriers() throw();
            explicit carriers();
        };
        
        //______________________________________________________________________
        //
        //
        //! helper to find a carrier for a type and a given infrastructure
        //
        //______________________________________________________________________
        template <typename T> inline
        const carrier &carrier_for(comms::infrastructure infra,
                                   const char           *where=NULL)
        {
            static const carriers &cdb = carriers::instance();
            return cdb.query<T>(infra,where);
        }
    }
    
}

#endif
