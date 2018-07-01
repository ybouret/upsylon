#ifndef Y_MEMORY_POOLED_INCLUDED
#define Y_MEMORY_POOLED_INCLUDED 1

#include "y/memory/carver.hpp"

namespace upsylon
{
    namespace memory
    {
        class pooled : public singleton<pooled>, public carver
        {
        public:
            

        private:
            inline explicit pooled() throw() : carver(Y_CHUNK_SIZE) {}
            inline virtual ~pooled() throw()
            {}
            friend class singleton<pooled>;
            static const at_exit::longevity life_time = object::provider::life_time - 1;
        };
    }
}

#endif

