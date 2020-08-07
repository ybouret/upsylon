//! \file

#ifndef Y_MEMORY_DYADIC_ALLOCATOR
#define Y_MEMORY_DYADIC_ALLOCATOR 1

#include "y/memory/small/dyadic-allocator.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon {

    namespace memory {

        class dyadic : public singleton<dyadic>, public small::dyadic_allocator
        {
        public:
            static const at_exit::longevity life_time;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dyadic);
            friend class singleton<dyadic>;
            virtual ~dyadic() throw();
            explicit dyadic();

        };

    }

}

#endif

