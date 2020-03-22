//! \file

#ifndef Y_JARGON_LEXICAL_UNIT_INCLUDED
#define Y_JARGON_LEXICAL_UNIT_INCLUDED 1

#include "y/jargon/token.hpp"

namespace upsylon {
 
    namespace Jargon {
     
        namespace Lexical {
         
            //! a token created by a lexical rule
            class Unit :
            public Object,
            public Context,
            public Token,
            public inode<Unit>
            {
            public:
                typedef core::list_of_cpp<Unit> List;
                
                virtual ~Unit() throw();
                
                explicit Unit(const Tag         &,
                              const Context     &,
                              const Char::Cache &) throw();
                
                const Tag label; //!< rule label
                
            private:
                Y_DISABLE_ASSIGN(Unit);
            };
            
        }
    }
    
}

#endif
