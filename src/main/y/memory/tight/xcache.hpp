
//! \file

#ifndef Y_MEMORY_TIGHT_XCACHE_INCLUDED
#define Y_MEMORY_TIGHT_XCACHE_INCLUDED 1

#include "y/memory/tight/objects.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {

            //__________________________________________________________________
            //
            //! eXtracted cache from objects
            //__________________________________________________________________
            class xcache
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit xcache(objects &, const size_t block_size); //!< extract access and shared arena
                virtual ~xcache() throw();                           //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                size_t block_size() const throw();      //!< shared.block_size
                void * acquire();                       //!< locked acquire
                void   release(void *) throw();         //!< locked release
                void * acquire_unlocked();              //!< for multiple acquires
                void   release_unlocked(void *) throw();//!< for mutliple releases

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                lockable &access; //!< synchronize objects access

            private:
                arena    &shared;
                Y_DISABLE_COPY_AND_ASSIGN(xcache);
            };

            //__________________________________________________________________
            //
            //! eXtracted cache of same types
            //__________________________________________________________________
            template <typename T>
            class xcache_of : public xcache
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup with sizeof(T)
                inline explicit xcache_of(objects &objs) : xcache(objs,sizeof(T))
                {
                }

                //! cleanup
                inline virtual ~xcache_of() throw()
                {
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! acquire/cast
                inline T   *acquire()             { return static_cast<T*>(xcache::acquire()); }

                //! release/cast
                inline void release(T *p) throw() { xcache::release( (void*)p ); }

                //! for multiple acquires/cast
                inline T   *acquire_unlocked()             {return static_cast<T*>(xcache::acquire_unlocked()); }

                //! for multiple releases/cast
                inline void release_unlocked(T *p) throw() { xcache::release_unlocked( (void*)p ); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(xcache_of);
            };

        }

    }

}

#endif

