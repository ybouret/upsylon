
//! \file

#ifndef Y_AP_ARCHON_INCLUDED
#define Y_AP_ARCHON_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/memory/tight/quarry-allocator.hpp"
#include "y/memory/tight/quarry.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    namespace yap {

        //______________________________________________________________________
        //
        //
        //! dedicated memory provider
        //
        //______________________________________________________________________
        class archon :
        public    singleton<archon>,
        private   memory::tight::quarry,
        public    memory::tight::quarry_allocator
        {
        public:
            Y_SINGLETON_DECL_WITH(at_exit::uttermost-1,archon); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(archon);
            explicit archon() throw();
            virtual ~archon() throw();
        };

    }

}


#endif

