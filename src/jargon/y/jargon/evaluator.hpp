//! \file

#ifndef Y_JARGON_EVALUATOR_INCLUDED
#define Y_JARGON_EVALUATOR_INCLUDED 1

#include "y/jargon/xnode.hpp"

namespace upsylon {
    
    namespace Jargon {

        class Evaluator : public Object
        {
        public:
            
            virtual ~Evaluator() throw();
            
            template <typename LABEL> inline
            explicit Evaluator(const LABEL &id) :
            label( Tags::Make(id) ),
            depth(0)
            {
            }
            
            void browse(const XNode &root);
            
            const Tag label;
        protected:
            int depth;
            std::ostream & indent(std::ostream &) const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Evaluator);
            void __browse(const XNode &);
            void onTerminal(const Tag &, const Token &);
            void onInternal(const Tag &, const size_t );
        };
        
    }
    
}

#endif

