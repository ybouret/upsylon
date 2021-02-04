//! \file
#ifndef Y_MEMORY_LOCKED_INCLUDED
#define Y_MEMORY_LOCKED_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace memory
    {
        
        class locked_area
        {
        public:
            explicit locked_area(const size_t desiredBytes);
            virtual ~locked_area() throw();

            const size_t bytes;
            void        *entry;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(locked_area);
        };
    }
}

#endif
