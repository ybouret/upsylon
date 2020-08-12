//! \file

#ifndef Y_MP_MEMORY_INCLUDED
#define Y_MP_MEMORY_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/memory/tight/quarry-allocator.hpp"
#include "y/memory/tight/quarry.hpp"
#include "y/type/ints.hpp"

namespace upsylon {
    
    namespace mpl {

        //______________________________________________________________________
        //
        //
        //! dedicated memory for mpl
        //
        //______________________________________________________________________
        class dispatcher :
        public    singleton<dispatcher>,
        private   memory::tight::quarry,
        public    memory::tight::quarry_allocator
        {
        public:
            Y_SINGLETON_DECL_WITH(at_exit::uttermost-1,dispatcher); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dispatcher);
            explicit dispatcher() throw();
            virtual ~dispatcher() throw();
        };

    }
    
}


#endif

