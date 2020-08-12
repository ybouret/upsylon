
//! \file

#ifndef Y_SIBYL_PROVIDER_INCLUDED
#define Y_SIBYL_PROVIDER_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/memory/tight/quarry-allocator.hpp"
#include "y/memory/tight/quarry.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    namespace sibyl {

        //______________________________________________________________________
        //
        //
        //! dedicated memory provider
        //
        //______________________________________________________________________
        class provider :
        public    singleton<provider>,
        private   memory::tight::quarry,
        public    memory::tight::quarry_allocator
        {
        public:
            Y_SINGLETON_DECL_WITH(at_exit::uttermost-1,provider); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(provider);
            explicit provider() throw();
            virtual ~provider() throw();
        };

    }

}


#endif

