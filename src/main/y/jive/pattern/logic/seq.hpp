
//! \file

#ifndef Y_PATTERN_LOGICAL_SEQ_INCLUDED
#define Y_PATTERN_LOGICAL_SEQ_INCLUDED 1

#include "y/jive/pattern/logic/logical.hpp"

namespace upsylon {

    namespace Jive
    {
        class Sequential : public Logical
        {
        public:
            virtual ~Sequential() throw();
            virtual void add(Pattern *) throw();
            virtual void harden();
            
        protected:
            explicit Sequential(const uint32_t) throw(); //!< setup
            explicit Sequential(const Sequential&);

        private:
            Y_DISABLE_ASSIGN(Sequential);
        };

    }

}

#endif
