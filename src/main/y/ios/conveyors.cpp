#include "y/ios/conveyors.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/code/round.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/spec.hpp"
#include "y/associative/xbe-key.hpp"
#include "y/exception.hpp"

namespace upsylon {
 
    namespace ios {
    
        namespace {
            
            typedef suffix_tree<conveyors::convoy> db_type;
            static uint64_t wksp[ Y_U64_FOR_ITEM(db_type) ];
            
            static inline db_type & __db() throw()
            {
                return *static_cast<db_type *>( aliasing::anonymous(wksp));
            }
            
            typedef xbe_key<uint8_t> db_key;
        }
                             
        conveyors:: ~conveyors() throw()
        {
            self_destruct( __db() );
        }
        
        
        conveyors:: conveyors()
        {
            Y_BZSET_STATIC(wksp);
            new ( aliasing::anonymous(wksp) ) db_type();
        }
        
        
        void conveyors:: throw_invalid_environment() const
        {
            throw exception("ios::conveyors: innvalid environment");
        }
        
        const conveyor & conveyors:: insert(const std::type_info   &t,
                                            const comms::medium     w,
                                            const convoy           &c)
        
        {
            static db_type &db   = __db();
            const  db_key   key(t,w);
            
            if(!db.insert_by(key,c))
            {
                const type_spec &ts = type_spec::declare(t);
                throw exception("ios::conveyors(multiple type <%s>", *ts.name());
                
            }
            return *c;
        }
        

        
        const conveyor * conveyors:: search(const std::type_info  &t,
                                            const comms::medium    w) const throw()
        {
            Y_LOCK(access);
            static db_type &db   = __db();
            const  db_key   key(t,w);
            const  convoy  *ppC  = db.search_by(key);
            if(ppC)
            {
                return & (**ppC);
            }
            else
            {
                return NULL;
            }
        }
        

        
    }
    
}
