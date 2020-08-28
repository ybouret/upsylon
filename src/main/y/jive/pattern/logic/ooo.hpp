//! \file

#ifndef Y_PATTERN_LOGICAL_OOO_INCLUDED
#define Y_PATTERN_LOGICAL_OOO_INCLUDED 1

#include "y/jive/pattern/logic/logical.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! for Out Of Order operations
        //
        //______________________________________________________________________
        class OutOfOrder : public Logical
        {
        public:
            virtual ~OutOfOrder() throw();
            virtual void append(Pattern *) throw();
            virtual void harden()          throw();
            
        protected:
            explicit OutOfOrder(const uint32_t) throw(); //!< setup
            explicit OutOfOrder(const OutOfOrder&);

        private:
            Y_DISABLE_ASSIGN(OutOfOrder);
        };

    }

}

#endif
