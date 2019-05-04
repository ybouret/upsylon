//! \file
#ifndef Y_MEMORY_DYADIC_INCLUDED
#define Y_MEMORY_DYADIC_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/longevities.hpp"

namespace upsylon
{
    namespace memory
    {
        //! specific allocator, based on internal vein
        class dyadic : public singleton<dyadic>, public allocator
        {
        public:
            virtual void *acquire(size_t &n);                    //!< allocator interface: acquire
            virtual void  release(void * &p, size_t &n) throw(); //!< allocator interface: release

        private:
            explicit dyadic() throw();
            virtual ~dyadic() throw();
            friend class singleton<dyadic>;
            static const at_exit::longevity life_time = longevity_for::memory_object-1;
        };
    }
}

#endif


