
//! \file

#ifndef Y_JARGON_PATTERN_OPERANDS_INCLUDED
#define Y_JARGON_PATTERN_OPERANDS_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! upgraded list of cloneable patters
        class Operands : public Pattern::List
        {
        public:
            explicit Operands() throw(); //!< setup
            virtual ~Operands() throw(); //!< cleanup
            Operands(const Operands &);  //!< copy by cloning
            
            
            static bool Alike(const Operands *, const Operands *) throw(); //!< test alike
            
            
        private:
            Y_DISABLE_ASSIGN(Operands);
        };
    }
    
}

#endif

