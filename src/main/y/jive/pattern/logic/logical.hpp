
//! \file

#ifndef Y_PATTERN_LOGICAL_INCLUDED
#define Y_PATTERN_LOGICAL_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {
        
        class Logical : public Pattern
        {
        public:
            Pattern::List operands;

            virtual ~Logical() throw();

            virtual size_t serialize(ios::ostream &fp) const;

            void load(ios::istream &p);  //!< read operands

            void add(Pattern *p) throw();

        protected:
            explicit Logical(const uint32_t) throw();
            explicit Logical(const Logical &);
            void vizLink(ios::ostream&) const;
            
        private:
            Y_DISABLE_ASSIGN(Logical);

        };

    }

}

#endif
