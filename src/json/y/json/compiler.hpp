
//! \file
#ifndef Y_JSON_COMPILER_INCLUDED
#define Y_JSON_COMPILER_INCLUDED 1

#include "y/json/value.hpp"
#include "y/jargon/parser.hpp"
#include "y/jargon/evaluator.hpp"
#include "y/hashing/mph.hpp"

namespace upsylon {
 
    namespace JSON {
      
        class Compiler :
        public Jargon::Parser,
        public Jargon::Evaluator
        {
        public:
            explicit Compiler();
            virtual ~Compiler() throw();
            
            Value                 value;
            Array                 vArray;
            vector<Pair>          vObject;
            const hashing::mperf  terminalHash;
            const hashing::mperf  internalHash;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            virtual void onBrowsing();
            virtual void onTerminal(const Jargon::Tag &, const Jargon::Token &);
            virtual void onInternal(const Jargon::Tag &, const size_t );
            virtual void onFinalize();
        };
        
    }
    
}

#endif

