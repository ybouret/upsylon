
//! \file

#ifndef Y_JARGON_CONTEXT_INCLUDED
#define Y_JARGON_CONTEXT_INCLUDED 1

#include "y/jargon/types.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Context
        {
        public:
            const Stamp    stamp;
            const unsigned line;
            const unsigned column;
            
            explicit Context(const Stamp  &) throw();
            explicit Context(const string &);
            explicit Context(const char   *);
            Context(const Context &) throw();
            virtual ~Context() throw();
            
        private:
            Y_DISABLE_ASSIGN(Context);
        };
    }
    
}

#endif
