//! \file
#ifndef Y_MEMORY_DYADIC_INCLUDED
#define Y_MEMORY_DYADIC_INCLUDED 1

#include "y/memory/vein.hpp"

namespace upsylon
{
    namespace memory
    {
        //! specific allocator
        class dyadic : public singleton<dyadic>, public vein
        {
        public:
            inline virtual void *acquire(size_t &n)
            {
                Y_LOCK(access);
                return vein::acquire(n);
            }

            inline virtual void release(void * &p, size_t &n) throw()
            {
                Y_LOCK(access);
                vein::release(p,n);
            }

        private:
            explicit dyadic() throw();
            virtual ~dyadic() throw();
            friend class singleton<dyadic>;
            static const at_exit::longevity life_time = object::life_time - 1;
        };
    }
}

#endif


