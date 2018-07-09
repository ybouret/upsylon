//! \file
#ifndef Y_MEMORY_NUGGETS_INCLUDED
#define Y_MEMORY_NUGGETS_INCLUDED 1

#include "y/memory/nugget.hpp"
#include "y/object.hpp"

namespace upsylon
{
    namespace memory
    {
        template <const size_t BLOCK_BITS>
        class nuggets
        {
        public:
            typedef nugget<BLOCK_BITS> nugget_type;
            static  const size_t       nuggets_per_page = (Y_CHUNK_SIZE - sizeof(void*))/sizeof(nugget_type);

            inline explicit nuggets() :
            available(0),
            content(),
            cached(),
            pages(),
            num_blocks( next_power_of_two<size_t>(Y_CHUNK_SIZE/nugget_type::block_size) ),
            chunk_size( num_blocks * nugget_type::block_size )
            {
            }

            inline virtual ~nuggets() throw()
            {
                if(pages.size)
                {
                    global &hmem = global::location();
                    for(nugget_type *node=content.tail;node;node=node->prev)
                    {
                        hmem.__free(node->data,node->bytes);
                    }
                    content.reset();
                    cached.reset();
                    while(pages.size)
                    {
                        hmem.__free(pages.query(),Y_CHUNK_SIZE);
                    }
                }
            }

            void *acquire()
            {

                if(!available)
                {
                    acquiring = create_nugget();
                }
                else
                {
                    assert(acquiring);
                }
                assert(acquiring);
                assert(acquiring->still_available);
                return acquiring->acquire();
            }
        private:
            struct page
            {
                page *next;
            };
            nugget_type               *acquiring;
            size_t                     available;
            core::list_of<nugget_type> content;
            core::pool_of<nugget_type> cached;
            core::pool_of<page>        pages;
            const size_t               num_blocks;
            const size_t               chunk_size;

            Y_DISABLE_COPY_AND_ASSIGN(nuggets);
            inline void create_nuggets()
            {
                nugget_type *node = io::cast<nugget_type>(pages.store(static_cast<page *>(global::instance().__calloc(1,Y_CHUNK_SIZE))),sizeof(void*));
                for(size_t i=0;i<nuggets_per_page;++i)
                {
                    (void)cached.store( node+i );
                }
            }

            nugget_type *create_nugget()
            {
                if(cached.size<=0) create_nuggets();
                void *data = memory::global::instance().__calloc(1,chunk_size);
                nugget_type *node = new ( cached.query() ) nugget_type(chunk_size,data);
                assert(num_blocks==node->still_available);
                available += num_blocks;
                content.push_back(node);
                return node;
            }
        };
    }
}


#endif

