//! \file

#ifndef Y_MP_MEMORY_INCLUDED
#define Y_MP_MEMORY_INCLUDED 1

#include "y/concurrent/singleton.hpp"
#include "y/memory/small/quarry-allocator.hpp"
#include "y/memory/small/quarry.hpp"
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
        private   memory::small::quarry,
        public    memory::small::quarry_allocator
        {
        public:
            //! life_time is just behind global
            static const at_exit::longevity life_time = limit_of<at_exit::longevity>::maximum-1;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(dispatcher);
            friend class singleton<dispatcher>;
            explicit dispatcher() throw();
            virtual ~dispatcher() throw();
        };

    }
    
}


#endif

