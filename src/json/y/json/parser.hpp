
//! \file
#ifndef Y_JSON_PARSER_INCLUDED
#define Y_JSON_PARSER_INCLUDED 1

#include "y/json/value.hpp"
#include "y/jargon/parser.hpp"
namespace upsylon {
 
    namespace JSON {
      
        class Parser : public Jargon::Parser
        {
        public:
            explicit Parser();
            virtual ~Parser() throw();
            
            Value value;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
        
    }
    
}

#endif

