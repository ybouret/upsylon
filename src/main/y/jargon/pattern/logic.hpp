//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_INCLUDED 1

#include "y/jargon/pattern/operands.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logic based pattern
        class Logic : public Pattern, public Operands
        {
        public:
            virtual ~Logic() throw(); //!< cleanup
            
        protected:
            explicit Logic(const uint32_t) throw(); //!< forward uuid
            Logic(const Logic &);                   //!< full copy
            size_t   srz(ios::ostream &fp) const;   //!< serialize
            void     viz(ios::ostream &fp) const;   //!< graphviz
            
        private:
            Y_DISABLE_ASSIGN(Logic);
            virtual size_t serialize( ios::ostream &) const;
        };
        
    }
    
}


#endif
