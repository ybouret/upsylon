//! \file

#ifndef Y_MATH_ADJUST_VARIABLE_INCLUDED
#define Y_MATH_ADJUST_VARIABLE_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon {
    
    namespace Adjust
    {
        //! variable interface
        class Variable : public counted_object
        {
        public:
            typedef intr_ptr<string,Variable> Pointer;
            
            const string name;
            
            virtual size_t index() const throw() = 0;
            virtual       ~Variable()    throw();
            
            
            Variable(const Variable &);
            const string & key() const throw();
            
        protected:
            explicit Variable(const string &);
            explicit Variable(const char   *);
            
        private:
            Y_DISABLE_ASSIGN(Variable);
        };
    }
}

#endif

