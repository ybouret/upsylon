//! \file

#ifndef Y_MEMORY_JOINT_WARD_INCLUDED
#define Y_MEMORY_JOINT_WARD_INCLUDED 1

#include "y/memory/joint/section.hpp"
#include "y/memory/tight/zcache.hpp"

namespace upsylon {

    namespace memory {

        namespace joint {

            //__________________________________________________________________
            //
            //
            //! cache for sections of different size, with a default one
            //
            //__________________________________________________________________
            class ward
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit ward(const size_t usr_chunk_size); //!< build with default chunk size
                virtual ~ward() throw();                    //!< cleanup

                void * acquire(size_t &n);                 //!< acquire for a block
                size_t chunk_size() const throw();         //!< default vein.block_size

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ward);
                section               *acquiring; //!< last acquisition
                core::list_of<section> S;         //!< all the sections
                tight::quarry          Q;         //!< cache for section memory
                tight::vein           &V;         //!< default vein for chunk_size
                tight::zcache<section> Z;         //!< cache of zombie sections

                //! find the vein that shall hold required bytes
                tight::vein &vein_for(const size_t required);
                section     *section_for(const size_t required);
            };


        }

    }
}

#endif
