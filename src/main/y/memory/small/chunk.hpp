//! \file

#ifndef Y_MEMORY_SMALL_CHUNK_INCLUDED
#define Y_MEMORY_SMALL_CHUNK_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/memory/ownership.hpp"

namespace upsylon {
    
    namespace memory {

        namespace small {

            //__________________________________________________________________
            //
            //
            //! chunk of memory for multiple contiguous objects
            //
            //__________________________________________________________________
            class chunk
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                // constructor: setup all parameters
                chunk(const size_t block_size,
                      void *       chunk_data,
                      const size_t chunk_size) throw();


                ~chunk() throw();

                //______________________________________________________________
                //
                // method
                //______________________________________________________________
                ownership owner_of(const void *addr) const throw(); //!< ownership of an address
                bool      owns(const void *addr)     const throw(); //!< check is this is the owner
                bool      is_empty()  const throw(); //!< still_available>=provided_number
                size_t    allocated() const throw(); //!< provided_number-still_available


                //! acquire a new block, still_available>0
                void     *acquire(const size_t block_size) throw();
                void      release(void *addr, const size_t block_size) throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                chunk         *next; //!< for list/poo;
                chunk         *prev; //!< for list
                uint8_t       *data; //!< first item
                const uint8_t *last; //!< first invalid item
                uint8_t        first_available; //!< bookeeping
                uint8_t        still_available; //!< bookeeping
                const uint8_t  provided_number; //!< initial count
                
                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________

                //! optimize the chunk size
                /**
                 optimize the required/boundary ratio, preferring to
                 decrease chunk_size than use a half empty memory
                 */
                static size_t max_chunk_size_for(const size_t block_size) throw();

                //! next power of two
                static size_t min_chunk_size_for(const size_t block_size) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(chunk);
                bool is_aligned(const void *addr, const size_t block_size) const throw();
            };

        }
    }
}

#endif
