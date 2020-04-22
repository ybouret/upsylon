#ifndef Y_JARGON_DIALECT_INCLUDED
#define Y_JARGON_DIALECT_INCLUDED 1

#include "y/jargon/parser.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Dialect : public Parser
        {
        public:
            //! setup the parser
            explicit Dialect();
            virtual ~Dialect() throw();
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dialect);
        };
        
    }
    
}


#endif
