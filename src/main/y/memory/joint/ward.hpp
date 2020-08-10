//! \file

#ifndef Y_MEMORY_JOINT_WARD_INCLUDED
#define Y_MEMORY_JOINT_WARD_INCLUDED 1

#include "y/memory/joint/section.hpp"
#include "y/memory/tight/zcache.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {


            class ward
            {
            public:
                explicit ward(const size_t usr_chunk_size);
                virtual ~ward() throw();

                const size_t chunk_size;

            private:
                tight::quarry          Q;
                tight::zcache<section> Z;
            };


        }

    }
}

#endif
