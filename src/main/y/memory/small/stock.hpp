
//! \file

#ifndef Y_MEMORY_SMALL_STOCK_INCLUDED
#define Y_MEMORY_SMALL_STOCK_INCLUDED 1

#include "y/memory/small/hoard.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            //__________________________________________________________________
            //
            //
            //! cache to locally and safely query/store items of same block_size
            //
            //__________________________________________________________________
            class stock
            {
            public:
                //! minimal valid block_size
                static const size_t  min_block_size = sizeof(void*);

                explicit stock(hoard &h); //!< setup with persistent hoard
                virtual ~stock() throw(); //!< cleanup


                void  *query();                    //!< query a block
                void   store(void *) throw();      //!< store a block
                void   reserve(size_t);            //!< reserve extra blocks
                void   release() throw();          //!< empty cache
                size_t available() const throw();  //!< cache size
                size_t block_size() const throw(); //!< common blocks size

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
