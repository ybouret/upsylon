//! \file
#ifndef Y_MEMORY_SMALL_OBJECT
#define Y_MEMORY_SMALL_OBJECT 1

#include "y/memory/blocks.hpp"
#include "y/memory/global.hpp"
#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    namespace memory
    {

        //! small object prototype
        template <size_t LIMIT_SIZE,size_t CHUNK_SIZE>
        class small_object
        {
        public:
            static const size_t limit_size = LIMIT_SIZE; //!< limit size of a small object
            static const size_t chunk_size = CHUNK_SIZE; //!< the common chunk size for allocation
            
            //! do nothing constructir
            inline explicit small_object() throw() {}

            //! do nothing destructor
            inline virtual ~small_object() throw() {}

            //! operator new
            static inline void *operator new(size_t block_size)
            {
                assert( block_size > 0 );
                static provider &mgr = provider::instance();
                return mgr.acquire(block_size);
            }

            //! operator delete, must support NULL
            static inline void operator delete(void *p, size_t block_size) throw()
            {
                assert(block_size>0);
                if( p )
                {
                    assert( provider::exists() );
                    static provider &mgr = provider::location();
                    mgr.release(p,block_size);
                }
            }

            //! array-like
            static inline void *operator new[] ( size_t block_size )
            {
                assert(block_size>0);
                static global &big = global::instance();
                return big.__calloc(1,block_size);
            }

            //! array-like delete
            static inline void  operator delete[] ( void *p, size_t block_size ) throw()
            {
                assert(block_size>0);
                if(p)
                {
                    assert(global::exists());
                    static global &big = global::location();
                    big.__free(p,block_size);
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

        private:
            class provider : public singleton<provider>
            {
            public:
                inline void *acquire( const size_t block_size )
                {
                    assert(block_size>0);
                    Y_LOCK(this->access);
                    if(block_size<=LIMIT_SIZE)
                    {
                        return blk.acquire(block_size);
                    }
                    else
                    {
                        static global &big = global::instance();
                        return big.__calloc(1,block_size);
                    }
                }

                inline void release(void *p, const size_t block_size) throw()
                {
                    Y_LOCK(this->access);
                    assert(p);
                    assert(block_size>0);
                    if(block_size<=LIMIT_SIZE)
                    {
                        blk.release(p,block_size);
                    }
                    else
                    {
                        assert(global::exists());
                        static global &big = global::location();
                        big.__free(p,block_size);
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(provider);
                friend class singleton<provider>;
                blocks blk;
                inline explicit provider() : blk(CHUNK_SIZE) {}
                inline virtual ~provider() throw() {}

            public:
                static const at_exit::longevity life_time = global::life_time - 1;
            };

        };

    }
}

#endif

