
//! \file

#ifndef Y_GFX_FILTERS_DB_INCLUDED
#define Y_GFX_FILTERS_DB_INCLUDED 1

#include "y/gfx/filters.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        
        typedef hash_set<string,shared_filters> filters_db_;
        
        class filters_db : public filters_db_, public singleton<filters_db>
        {
        public:
            void use( filters *f );
            
            const shared_filters & operator[](const string &) const;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(filters_db);
            Y_SINGLETON_DECL_WITH(99,filters_db);

            virtual ~filters_db() throw();
            explicit filters_db();
            
        };
    }
    
}


#endif

