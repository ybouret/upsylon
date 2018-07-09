//! \file
#ifndef Y_MEMORY_NUGGETS_INCLUDED
#define Y_MEMORY_NUGGETS_INCLUDED 1

#include "y/memory/nugget.hpp"
#include "y/object.hpp"
#include <iostream>

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
            acquiring(0),
            releasing(0),
            available(0),
            content(),
            cached(),
            pages(),
            num_blocks( next_power_of_two<size_t>(Y_CHUNK_SIZE/nugget_type::block_size) ),
            chunk_size( num_blocks * nugget_type::block_size )
            {
                //assert(chunk_size/nugget_type::block_size==num_blocks);
                std::cerr << "New Nuggets(BLOCK_BITS=" << BLOCK_BITS <<")" << std::endl;
            }

            inline size_t get_block_bits() const throw() { return BLOCK_BITS; }
            inline size_t get_num_blocks() const throw() { return num_blocks; }


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
                    if(acquiring->still_available<=0)
                    {
                        // there is some place block somewhere
                        nugget_type *lo = acquiring->prev;
                        nugget_type *up = acquiring->next;
                        while(lo&&up)
                        {
                            if(lo->still_available)
                            {
                                acquiring = lo;
                                goto ACQUIRE;
                            }

                            if(up->still_available)
                            {
                                acquiring = up;
                                goto ACQUIRE;
                            }
                            lo = lo->prev;
                            up = up->next;
                        }

                        while(lo)
                        {
                            if(lo->still_available)
                            {
                                acquiring = lo;
                                goto ACQUIRE;
                            }
                            lo=lo->prev;
                        }

                        while(up)
                        {
                            if(up->still_available)
                            {
                                acquiring = up;
                                goto ACQUIRE;
                            }
                            up = up->next;
                        }

                        // never get here
                        assert( die("invalid bookeeping" ) );
                    }
                }
            ACQUIRE:
                assert(acquiring);
                assert(available);
                assert(acquiring->still_available);
                --available;
                return acquiring->acquire();
            }

            //! release a previously allocated block of memory
            void release(void *p) throw()
            {
                assert(p);
                assert(acquiring||die("no previous acquire"));
                if(!releasing)
                {
                    releasing = acquiring;
                }

                switch( releasing->whose(p) )
                {
                    case owned_by_this:
                        goto RELEASE; // cached

                    case owned_by_prev:
                        while(0!=(releasing=releasing->prev))
                        {
                            if(releasing->owns(p)) goto RELEASE;
                        }
                        break;

                    case owned_by_next:
                        while(0!=(releasing=releasing->next))
                        {
                            if(releasing->owns(p)) goto RELEASE;
                        }
                        break;
                }

                assert( die("release: invalid bookeeping") );

            RELEASE:
                assert(releasing);
                assert(releasing->owns(p));
                releasing->release(p);
                ++available;
            }

        private:
            struct page
            {
                page *next;
            };
            nugget_type               *acquiring;
            nugget_type               *releasing;
            size_t                     available;
            core::list_of<nugget_type> content;
            core::pool_of<nugget_type> cached;
            core::pool_of<page>        pages;
            const size_t               num_blocks;
            const size_t               chunk_size;

            Y_DISABLE_COPY_AND_ASSIGN(nuggets);
            inline void create_nuggets()
            {
                assert(cached.size<=0);
                nugget_type *node = io::cast<nugget_type>(pages.store(static_cast<page *>(global::instance().__calloc(1,Y_CHUNK_SIZE))),sizeof(void*));
                for(size_t i=0;i<nuggets_per_page;++i)
                {
                    (void)cached.store( node+i );
                }
            }

            nugget_type *create_nugget()
            {
                if(cached.size<=0)
                {
                    create_nuggets();
                }
                assert(cached.size>0);
                void        *data = memory::global::instance().__calloc(1,chunk_size);
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

