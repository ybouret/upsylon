//! \file
#ifndef Y_IPSO_GHOSTS_INCLUDED
#define Y_IPSO_GHOSTS_INCLUDED 1

#include "y/ipso/ghost.hpp"

namespace upsylon
{
    namespace ipso
    {

        class ghosts
        {
        public:
            ghost source; //!< 'inner' zone, read and send
            ghost target; //!< 'outer' zone, recieve and write

            virtual ~ghosts() throw();

            template <typename LAYOUT>
            inline ghosts(const LAYOUT &parted_layout,
                          const int     source_peer,
                          const LAYOUT &source_layout,
                          const int     target_peer,
                          const LAYOUT &target_layout,
                          const size_t block_size = 0 ) :
            source(parted_layout,source_layout,source_peer,block_size),
            target(parted_layout,target_layout,target_peer,block_size)
            {

            }

            void ensure( const size_t block_size );

            //! directly write into target date from source
            void local_exchange( field_info &F ) const throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ghosts);
        };

    }
}

#endif

