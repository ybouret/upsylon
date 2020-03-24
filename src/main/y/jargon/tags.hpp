//! \file

#ifndef Y_JARGON_TAGS_INCLUDED
#define Y_JARGON_TAGS_INCLUDED 1

#include "y/jargon/types.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Tags : public suffix_tree<Tag>
        {
        public:
            explicit Tags();
            virtual ~Tags() throw();
            string  *query( const string &s );
            string  *query( const char   *s );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tags);
        };
        
    }
    
}

#endif

