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
                typedef tight::quarry          quarry_type; //!< alias
                typedef core::list_of<section> sections;    //!< alias
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit ward(const size_t usr_chunk_size); //!< build with default chunk size
                virtual ~ward() throw();                    //!< cleanup

                void * acquire_block(size_t &n);           //!< acquire a block of size>=n
                void   release_block(void * &p, size_t &n) throw(); //!< release a block
                size_t chunk_size() const throw();         //!< default vein.block_size

                const quarry_type & _quarry()   const throw(); //!< access
                const sections    & _sections() const throw(); //!< access

                friend std::ostream & operator<<( std::ostream &os, const ward &w);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ward);
                section               *acquiring; //!< last acquisition
                section               *empty_one; //!< if one empty section
                sections               S;         //!< all the sections
                quarry_type            Q;         //!< cache for section memory
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
