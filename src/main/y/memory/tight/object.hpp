
//! \file

#ifndef Y_MEMORY_TIGHT_OBJECT_INCLUDED
#define Y_MEMORY_TIGHT_OBJECT_INCLUDED 1

#include "y/memory/tight/objects.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {

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
                // new/delete pairs
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
                static inline void  operator delete[] (void *p, size_t block_size) throw()
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

                //! return objects prototype
                static inline objects &proto()
                {
                    static objects &self = supply::instance();
                    return self;
                }

                static inline bool is_tight(const void *block_addr, size_t &block_size) throw()
                {
                    if(supply::exists())
                    {
                        static supply &mgr = supply::location();
                        return mgr.owns(block_addr,block_size);
                    }
                    else
                    {
                        block_size = 0;
                        return false;
                    }
                }

                //! acquire 1<<block_exp2
                static inline void *dyadic_acquire(const size_t block_exp2)
                {
                    static supply &mgr = supply::instance();
                    return mgr.dyadic_acquire(block_exp2);
                }

                //! release 1<<exp2
                static inline void dyadic_release(void *addr,const size_t block_exp2) throw()
                {
                    assert(supply::exists());
                    assert(NULL!=addr);
                    static supply &mgr = supply::location();
                    mgr.dyadic_release(addr,block_exp2);
                }

                //______________________________________________________________
                //
                //! parametric singleton
                //______________________________________________________________
                class supply : public objects, public singleton<supply>
                {
                public:
                    static const char * const       call_sign; //! =id
                    static const at_exit::longevity life_time = at_exit::uttermost-3; //!< before global
                    
                private:
                    friend class singleton<supply>;
                    Y_DISABLE_COPY_AND_ASSIGN(supply);
                    
                    inline explicit supply() :
                    objects(this->access,chunk_size,limit_size,id,sizeof(id)),
                    singleton<supply>()
                    {
                    }

                    inline virtual ~supply() throw()
                    {}

                    static char id[64];
                };
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(object);

            public:
                static const at_exit::longevity life_time = supply::life_time; //!< repeat of supply
            };

            template <size_t CHUNK_SIZE,size_t LIMIT_SIZE>
            char object<CHUNK_SIZE,LIMIT_SIZE>::supply::id[];

            //! global call_sign points to local-single id
            template <size_t CHUNK_SIZE,size_t LIMIT_SIZE>
            const char * const object<CHUNK_SIZE,LIMIT_SIZE>::supply::call_sign = object<CHUNK_SIZE,LIMIT_SIZE>::supply::id;
        }

    }

}

#endif
