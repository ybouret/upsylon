//! \file

#ifndef Y_MEMORY_TIGHT_CHUNK_INCLUDED
#define Y_MEMORY_TIGHT_CHUNK_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/memory/ownership.hpp"
#include <iosfwd>

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

                //! constructor: setup all parameters
                /**
                 \param block_size the block_size allocated by this chunk
                 \param chunk_data persistent data, provided by user
                 \param chunk_size bytes of user's data
                 */
                chunk(const size_t block_size,
                      void *       chunk_data,
                      const size_t chunk_size) throw();


                //! cleanup
                ~chunk() throw();

                //______________________________________________________________
                //
                // method
                //______________________________________________________________
                ownership whose(const void *addr)    const throw(); //!< ownership of an address
                bool      owns(const void *addr)     const throw(); //!< check is this is the owner
                bool      is_empty()                 const throw(); //!< still_available>=provided_number
                size_t    allocated()                const throw(); //!< provided_number-still_available


                //! acquire a new block, still_available>0
                void     *acquire(const size_t block_size) throw();

                //! release a preivously acquire block
                void      release(void *addr, const size_t block_size) throw();

                //! display
                friend std::ostream & operator<<(std::ostream &, const chunk&);

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                chunk         *next;            //!< for list/pool
                chunk         *prev;            //!< for list
                uint8_t       *data;            //!< first item
                const uint8_t *last;            //!< first invalid item
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

                //! next_power_of_two(block_size)
                static size_t min_chunk_size_for(const size_t block_size) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(chunk);
                bool is_aligned(const void *addr, const size_t block_size) const throw();
            };

        }
    }
}

#endif

