//! \file

#ifndef Y_JARGON_EVALUATOR_INCLUDED
#define Y_JARGON_EVALUATOR_INCLUDED 1

#include "y/jargon/xnode.hpp"

namespace upsylon {
    
    namespace Jargon {

        class Evaluator : public Object
        {
        public:
            const Tag label;
            
            virtual ~Evaluator() throw();
            
            template <typename LABEL> inline
            explicit Evaluator(const LABEL &id) :
            label( Tags::Make(id) ),
            depth(0)
            {
            }
            
            
            
        protected:
            unsigned depth;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Evaluator);
            void __onTerminal(const string &, const Token &);
            void __onInternal(const string &);
        };
        
    }
    
}

#endif

