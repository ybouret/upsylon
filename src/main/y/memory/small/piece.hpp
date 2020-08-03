//! \file

#ifndef Y_MEMORY_PIECE_INCLUDED
#define Y_MEMORY_PIECE_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/memory/ownership.hpp"

namespace upsylon {
    
    namespace memory {

        class piece
        {
        public:

            piece         *next;
            piece         *prev;
            uint8_t       *data;
            const uint8_t *last;
            uint8_t        first_available; //!< bookeeping
            uint8_t        still_available; //!< bookeeping
            const uint8_t  provided_number; //!< initial count

            piece(const size_t block_size,
                  void *       chunk_data,
                  const size_t chunk_size) throw();

            ~piece() throw();

            ownership owner_of(const void *addr) const throw();
            bool      owns(const void *addr)     const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(piece);
        };
       
    }
}

#endif

