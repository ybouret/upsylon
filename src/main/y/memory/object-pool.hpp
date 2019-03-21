//! \file
#ifndef Y_MEMORY_OBJECT_POOL_INCLUDED
#define Y_MEMORY_OBJECT_POOL_INCLUDED 1

#include "y/object.hpp"
#include "y/core/pool.hpp"

namespace upsylon
{
    namespace memory
    {

        template <typename NODE>
        class object_pool : public core::pool_of<NODE>
        {
        public:
            inline explicit object_pool() throw() : core::pool_of<NODE>() {}
            inline virtual ~object_pool() throw() { clear(); }

            inline NODE *fetch() { return (this->size>0) ? this->query() : object::acquire1<NODE>(); }

            inline void clear() throw()
            {
                while(this->size)
                {
                    NODE *node = this->query();
                    object::release1<NODE>( node );
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(object_pool);
        };

    }
}

#endif

