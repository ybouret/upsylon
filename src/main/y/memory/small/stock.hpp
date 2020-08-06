
//! \file

#ifndef Y_MEMORY_SMALL_STOCK_INCLUDED
#define Y_MEMORY_SMALL_STOCK_INCLUDED 1

#include "y/memory/small/hoard.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            class stock
            {
            public:
                static const size_t  min_block_size = sizeof(void*);

                //! setup with persistent hoard
                explicit stock( hoard &h );
                virtual ~stock() throw();

                size_t block_size() const throw();

                void *query();
                void  store(void *) throw();
                void  reserve(size_t);
                void  release() throw();
                size_t available() const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(stock);
                struct node_t { node_t *next; };
                hoard                &proxy;
                core::pool_of<node_t> nodes;
            };

        }

    }

}

#endif
