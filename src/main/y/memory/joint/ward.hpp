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


            private:
                section               *acquiring;
                section               *releasing;
                section               *empty_one;
                core::list_of<section> S;
                tight::quarry          Q;
            public:
                const size_t           chunk_size; //!< default chunk_size to get
            private:
                tight::vein           &V;          //!< default vein for chunk_size
                tight::zcache<section> Z;

                tight::vein &vein_for(const size_t required);

            };


        }

    }
}

#endif
