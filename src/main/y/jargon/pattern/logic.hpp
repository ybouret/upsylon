//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_INCLUDED 1

#include "y/jargon/pattern/operands.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logical operations based pattern
        class Logical : public Pattern, public Operands
        {
        public:
            virtual ~Logical() throw();            //!< cleanup
            
        protected:
            explicit Logical(const uint32_t) throw();   //!< forward uuid
            Logical(const Logical &);                   //!< full copy
            size_t   srz(ios::ostream &fp) const;       //!< serialize
            void     viz(ios::ostream &fp) const;       //!< graphviz
            
        private:
            Y_DISABLE_ASSIGN(Logical);
            virtual size_t serialize( ios::ostream &) const;
            
        public:
            //! return head pattern/delete logical if only one item
            static Pattern *Compact(Logical *p);
        };
        
    }
    
}


#endif
