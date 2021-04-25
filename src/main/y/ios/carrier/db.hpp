

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
            typedef hashing::type_info_hasher<hashing::fnv>          hasher;   //!< direct hasher
            typedef hash_set<std::type_info,carrier::pointer,hasher> db_type;  //!< alias
            typedef db_type::const_iterator                          iterator; //!< alias
            
           
            template <typename T, template <class> class CARRIER>
            static inline void record(db_type &db)
            {
                const carrier::pointer ptr = new CARRIER<T>();
                (void) db.insert(ptr);
            }
            
            template <typename T>
            inline void record()
            {
                record<T,primary_carrier>( aliasing::_(homogeneous) );
                record<T,network_carrier>( aliasing::_(distributed) );
            }
            
            template <
            template <class> class TUPLE,
            typename               T,
            template <class> class CARRIER>
            static inline void record_tuple(db_type &db)
            {
                const carrier::pointer ptr = new tuple_carrier<TUPLE,T,CARRIER>();
                (void) db.insert(ptr);
            }
            
            template <
            template <class> class TUPLE,
            typename               T
            >
            inline void record_tuple()
            {
                record_tuple<TUPLE,T,primary_carrier>( aliasing::_(homogeneous) );
                record_tuple<TUPLE,T,network_carrier>( aliasing::_(distributed) );
            }
            
            template <typename T>
            inline void use()
            {
                const carrier::pointer ptr = new derived_carrier<T>();
                (void) aliasing::_(distributed).insert(ptr);
            }
            
            
            const db_type homogeneous; //!< database of homogeneous carriers
            const db_type distributed; //!< database of distributed carriers
            

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(carriers);
            Y_SINGLETON_DECL_WITH(10,carriers);
            virtual ~carriers() throw();
            explicit carriers();
       
            
        };
        
        
    }
    
}

#endif
