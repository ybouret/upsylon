
//! \file
#ifndef Y_JSON_COMPILER_INCLUDED
#define Y_JSON_COMPILER_INCLUDED 1

#include "y/json/value.hpp"
#include "y/jargon/parser.hpp"
#include "y/jargon/evaluator.hpp"

namespace upsylon {
 
    namespace JSON {
      
        class Compiler :
        public Jargon::Parser,
        public Jargon::Evaluator
        {
        public:
            explicit Compiler();
            virtual ~Compiler() throw();
            
            Value value;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            virtual void onBrowsing();
            
        };
        
    }
    
}

#endif

