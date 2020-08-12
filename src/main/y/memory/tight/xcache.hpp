
//! \file

#ifndef Y_MEMORY_TIGHT_XCACHE_INCLUDED
#define Y_MEMORY_TIGHT_XCACHE_INCLUDED 1

#include "y/memory/tight/objects.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {

            //! eXtracted cache
            class xcache
            {
            public:
                //! extract access and shared arena
                explicit xcache(objects &objs, const size_t block_size);


                //! cleanup
                virtual ~xcache() throw();

                //! shared.block_size
                size_t   block_size() const throw();


                void *acquire();               //!< locked acquire
                void  release(void *) throw(); //!< locked release

                lockable &access; //!< synchronize objects access

                void *acquire_unlocked();
                void  release_unlocked(void *addr) throw();

            private:
                arena    &shared;
                Y_DISABLE_COPY_AND_ASSIGN(xcache);
            };

            template <typename T>
            class xcache_of : public xcache
            {
            public:

                inline explicit xcache_of(objects &objs) : xcache(objs,sizeof(T))
                {
                }

                inline virtual ~xcache_of() throw()
                {
                }

                inline T   *acquire() { return static_cast<T*>(xcache::acquire()); }
                inline void release(T *p) throw() { xcache::release( (void*)p ); }

                inline T   *acquire_unlocked()             {return static_cast<T*>(xcache::acquire_unlocked()); }
                inline void release_unlocked(T *p) throw() { xcache::release_unlocked( (void*)p ); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(xcache_of);
            };

        }

    }

}

#endif

