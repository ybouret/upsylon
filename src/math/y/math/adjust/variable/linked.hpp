//! \file

#ifndef Y_MATH_ADJUST_VARIABLE_LINKED_INCLUDED
#define Y_MATH_ADJUST_VARIABLE_LINKED_INCLUDED 1

#include "y/math/adjust/variable.hpp"

namespace upsylon {
    
    namespace Adjust {
        
        class LinkedVariable : public Variable
        {
        public:
            const Variable::Pointer link;
            
            explicit LinkedVariable(const string &n, const Variable::Pointer &l);
            explicit LinkedVariable(const char   *n, const Variable::Pointer &l);

            virtual ~LinkedVariable() throw();
            LinkedVariable(const LinkedVariable &other);
            
        private:
            Y_DISABLE_ASSIGN(LinkedVariable);
        };
        
    }
}

#endif
