//! \file

#ifndef Y_MEMORY_SMALL_HOARD_INCLUDED
#define Y_MEMORY_SMALL_HOARD_INCLUDED 1

#include "y/lockable.hpp"


namespace upsylon {

    namespace memory {

        namespace small {

            class hoard
            {
            public:
                virtual ~hoard() throw();

                lockable     &access;     //!< shared lock
                const size_t  block_size; //!< common block size

                void *query();
                void  store(void *) throw();

            protected:
                explicit hoard(lockable &l, const size_t bs) throw();

            private:
                virtual void *on_query()               =0;
                virtual void  on_store(void *) throw() =0;
                Y_DISABLE_COPY_AND_ASSIGN(hoard);
            };
            
        }
    }
}

#endif


