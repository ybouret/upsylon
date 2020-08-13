
//! \file

#ifndef Y_MEMORY_TIGHT_OBJECTS_INCLUDED
#define Y_MEMORY_TIGHT_OBJECTS_INCLUDED 1

#include "y/memory/tight/blocks.hpp"
#include "y/lockable.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {
        
            //__________________________________________________________________
            //
            //
            //! handle all objects memory
            //
            //__________________________________________________________________
            class objects
            {
            public:
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                
                //! setup objets
                /**
                 \param sync synchronisation
                 \param chunk_size desired chunk_size for blocks
                 \param limit_size for blocks, extended to vein::small_size
                 \param buffer to format an id
                 \param buflen size of buffer
                 */
                explicit objects(lockable     &sync,
                                 const size_t chunk_size,
                                 const size_t limit_size,
                                 char        *buffer = 0,
                                 const size_t buflen = 0);
                
                //! cleanup
                virtual ~objects() throw();

               
               
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                
                //! locked Blocks.acquire(block_size)
                void *acquire(const size_t block_size);
                
                //! locked Blocks.release(block_size)
                void  release(void *addr, const size_t block_size) throw();

                //! forward call to global allocator
                void *global_acquire(const size_t global_bytes);
                
                //! forward call to global allocator
                void  global_release(void *addr, const size_t global_bytes) throw();

                //! any 2^n block size, using blocks.arena for smaller one
                void *dyadic_acquire(const size_t block_exp2);
                
                //! release a 2^n block
                void  dyadic_release(void *addr, const size_t block_exp2) throw();

                //! locked optimization or Quarry
                void  optimize() throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                lockable &Access; //!< persistent synchro object
                quarry    Quarry; //!< for all 2^n allocation
                blocks    Blocks; //!< for 1..limit_size + caches

            private:
                Y_DISABLE_COPY_AND_ASSIGN(objects);
                arena *little[vein::min_exp2];
                void format(char        *buffer,
                            const size_t buflen,
                            const size_t cs,
                            const size_t ls) const throw();
            };

        }

    }

}
#endif
