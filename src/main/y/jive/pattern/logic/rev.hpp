//! \file

#ifndef Y_PATTERN_LOGICAL_REV_INCLUDED
#define Y_PATTERN_LOGICAL_REV_INCLUDED 1

#include "y/jive/pattern/logic/logical.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! for reworkable operations
        //
        //______________________________________________________________________
        class Reworkable : public Logical
        {
        public:
            virtual ~Reworkable() throw(); //!< cleanup
            virtual void rework() throw() = 0; //!< reworking
            
        protected:
            explicit Reworkable(const uint32_t) throw(); //!< setup
            explicit Reworkable(const Reworkable&);      //!< copy

        private:
            Y_DISABLE_ASSIGN(Reworkable);
        };

    }

}

#endif
