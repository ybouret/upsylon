
//! \file

#ifndef Y_JARGON_PATTERN_OPERANDS_INCLUDED
#define Y_JARGON_PATTERN_OPERANDS_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Operands : public Pattern::List
        {
        public:
            explicit Operands() throw();
            virtual ~Operands() throw();
            Operands(const Operands &);
            
            static bool Alike( const Operands &lhs, const Operands &rhs ) throw();
            
        private:
            Y_DISABLE_ASSIGN(Operands);
        };
    }
    
}

#endif

