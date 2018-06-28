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


        private:
            explicit global() throw();
            virtual ~global() throw();
            Y_DISABLE_COPY_AND_ASSIGN(global);
            friend class singleton<global>;

            virtual void * __acquire( size_t &n );
            virtual void   __release( void * &p, size_t &n ) throw();
           

        public:
            //! the longest life_time
            static const at_exit::longevity life_time = limit_of<at_exit::longevity>::maximum;
        };
    }
}

#endif

