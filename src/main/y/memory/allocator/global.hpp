//! \file
#ifndef Y_MEMORY_GLOBAL_INCLUDED
#define Y_MEMORY_GLOBAL_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/type/ints.hpp"

namespace upsylon
{

    namespace  memory
    {
        //! global calloc/free allocator
        class global : public singleton<global>, public allocator
        {
        public:
            int64_t allocated; //!< bookeeping
            
            //! legacy calloc, with monitoring
            void *__calloc(const size_t count, const size_t size);

            //! legacy free, with monitoring
            void  __free(void *p,const size_t bytes) throw();

            //! using calloc
            virtual void * acquire( size_t &n );

            //! using free
            virtual void   release( void * &p, size_t &n ) throw();

            //! proxy class
            class allocator : public memory::allocator
            {
            public:
                inline explicit allocator() throw() {}
                inline virtual ~allocator() throw() {}
                inline virtual  void *acquire(size_t &n) { return global::instance().acquire(n); }
                inline virtual  void  release(void * &p, size_t &n) throw() { if(p) global::location().release(p,n); }
            private:
                Y_DISABLE_COPY_AND_ASSIGN(allocator);
            };
        private:
            explicit global() throw();
            virtual ~global() throw();
            Y_DISABLE_COPY_AND_ASSIGN(global);

        public:
            Y_SINGLETON_DECL_WITH( at_exit::uttermost,global);
        };
    }
}

#endif

