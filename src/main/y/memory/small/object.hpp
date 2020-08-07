
//! \file

#ifndef Y_MEMORY_SMALL_OBJECT_INCLUDED
#define Y_MEMORY_SMALL_OBJECT_INCLUDED 1

#include "y/memory/small/objects.hpp"
#include "y/memory/small/hoard.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/longevities.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            //__________________________________________________________________
            //
            //
            //! small object prototype
            //
            //__________________________________________________________________
            template <size_t CHUNK_SIZE,size_t LIMIT_SIZE>
            class object
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                static const size_t chunk_size = CHUNK_SIZE; //!< the common chunk size for allocation
                static const size_t limit_size = LIMIT_SIZE; //!< limit size of a small object

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! do nothing constructor
                inline explicit object() throw() {}

                //! do nothing destructor
                inline virtual ~object() throw() {}

                //______________________________________________________________
                //
                // new/deleta pairs
                //______________________________________________________________

                //! operator new
                static inline void *operator new(size_t block_size)
                {
                    assert(block_size>0);
                    static supply &mgr = supply::instance();
                    return mgr.acquire(block_size);
                }

                //! operator delete, must support NULL
                static inline void operator delete(void *p, size_t block_size) throw()
                {
                    assert(block_size>0);
                    if(NULL!=p)
                    {
                        assert(supply::exists());
                        static supply &mgr = supply::location();
                        mgr.release(p,block_size);
                    }
                }

                //! array-like
                static inline void *operator new[] (size_t block_size)
                {
                    assert(block_size>0);
                    static supply &mgr = supply::instance();
                    return mgr.global_acquire(block_size);
                }

                //! array-like delete
                static inline void  operator delete[] ( void *p, size_t block_size ) throw()
                {
                    assert(block_size>0);
                    if(NULL!=p)
                    {
                        assert(supply::exists());
                        static supply &mgr = supply::location();
                        mgr.global_release(p,block_size);
                    }
                }

                //! placement new
                static inline void *operator new( size_t
#if !defined(NDEBUG)
                                                 block_size
#endif
                                                 , void *addr ) throw()
                {
                    assert(block_size>0);
                    assert(addr!=NULL);
                    return addr;
                }

                //! placement delete
                static inline void  operator delete( void *, void *) throw() {}

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________

                //! acquire memory for exactly one object
                template <typename T>
                static inline T *acquire1()
                {
                    return static_cast<T*>(operator new(sizeof(T)));
                }

                //! release memory for exactly one prvisouly acquired object
                template <typename T>
                static inline void release1( T * &p ) throw()
                {
                    operator delete(p,sizeof(T));
                    p = 0;
                }

                static inline objects &proto()
                {
                    static objects &self = supply::instance();
                    return self;
                }
                
                //______________________________________________________________
                //
                //! parametric singleton
                //______________________________________________________________
                class supply : public singleton<supply>, public objects
                {
                public:
                    static const at_exit::longevity life_time = longevity_for::memory_object;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(supply);
                    friend class singleton<supply>;

                    inline explicit supply() :
                    singleton<supply>(), objects(this->access,chunk_size,limit_size)
                    {
                    }

                    inline virtual ~supply() throw()
                    {}

                };
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(object);

            public:
                static const at_exit::longevity life_time = supply::life_time; //! repeat
            };
        }

    }

}

#endif
