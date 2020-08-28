
//! \file

#ifndef Y_PATTERN_LOGICAL_SEQ_INCLUDED
#define Y_PATTERN_LOGICAL_SEQ_INCLUDED 1

#include "y/jive/pattern/logic/logical.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! for Sequential operations
        //
        //______________________________________________________________________
        class Sequential : public Logical
        {
        public:
            virtual ~Sequential() throw();
            virtual void append(Pattern *) throw();
            virtual void harden()          throw();

        protected:
            explicit Sequential(const uint32_t) throw(); //!< setup
            explicit Sequential(const Sequential&);

        private:
            Y_DISABLE_ASSIGN(Sequential);
        };

    }

}

#endif
