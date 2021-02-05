//! \file
#ifndef Y_MEMORY_LOCKED_INCLUDED
#define Y_MEMORY_LOCKED_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! locked area
        //
        //______________________________________________________________________
        class locked_area
        {
        public:

            explicit locked_area(const size_t desiredBytes); //!< initialize
            virtual ~locked_area() throw();                  //!< cleanup

            const size_t bytes; //!< locked bytes
            void        *entry; //!< address of first locked byte


        private:
            Y_DISABLE_COPY_AND_ASSIGN(locked_area);
        };
    }
}

#endif
