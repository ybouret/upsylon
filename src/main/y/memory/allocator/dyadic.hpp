//! \file
#ifndef Y_MEMORY_DYADIC_INCLUDED
#define Y_MEMORY_DYADIC_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/type/ints.hpp"

namespace upsylon
{
    namespace memory
    {
        //! specific allocator
        class dyadic : public singleton<dyadic>, public allocator
        {
        public:
            virtual void *acquire(size_t &n);                    //!< allocator interface: acquire
            virtual void  release(void * &p, size_t &n) throw(); //!< allocator interface: release

        private:
            explicit dyadic();
            virtual ~dyadic() throw();
            void    *impl;
            friend class singleton<dyadic>;
            static const at_exit::longevity life_time = limit_of<at_exit::longevity>::maximum-2; //!< behin object
        };
    }
}

#endif


