//! \file
#ifndef Y_MEMORY_POOLED_INCLUDED
#define Y_MEMORY_POOLED_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/type/ints.hpp"

namespace upsylon
{
    namespace memory
    {
        //! pooled memory allocator
        class pooled : public singleton<pooled>, public allocator
        {
        public:
            virtual void *acquire( size_t &n ); //!< allocator interface: acquire
            virtual void  release(void * &p, size_t &n) throw(); //!< allocator interface: release
            
        private:
            explicit pooled() throw();
            virtual ~pooled() throw();

            friend class singleton<pooled>;
            
        public:
            //! come right before object::life_time
            static const at_exit::longevity life_time = limit_of<at_exit::longevity>::maximum - 2;
        };
    }
}

#endif

