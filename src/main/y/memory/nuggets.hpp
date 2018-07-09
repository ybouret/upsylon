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
        class nuggets_manager
        {
        public:
            inline virtual ~nuggets_manager() throw() {}

            virtual void *acquire() = 0;
            virtual void  release(void *p) throw() = 0;
            virtual size_t get_num_blocks() const throw() = 0;
            virtual size_t get_block_bits() const throw() = 0;
            virtual size_t get_block_size() const throw() = 0;
            virtual size_t get_chunk_size() const throw() = 0;
        protected:
            inline explicit nuggets_manager() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(nuggets_manager);
        };

        template <const size_t BLOCK_BITS>
        class nuggets : public nuggets_manager
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
                std::cerr << "nuggets(BLOCK_BITS=" << BLOCK_BITS <<",BLOCK_BYTES=" << nugget_type::block_size << ")" << std::endl;
            }

            inline virtual size_t get_block_bits() const throw() { return nugget_type::block_bits; }
            inline virtual size_t get_block_size() const throw() { return nugget_type::block_size; }
            inline virtual size_t get_num_blocks() const throw() { return num_blocks; }
            inline virtual size_t get_chunk_size() const throw() { return num_blocks; }


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

            inline virtual void *acquire()
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
            inline virtual void release(void *p) throw()
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

            inline bool memory_is_ordered() const throw()
            {
                if( content.size > 0 )
                {
                    for(const nugget_type *scan = content.head; scan->next; scan=scan->next )
                    {
                        if(scan->data>=scan->next->data)
                            return false;
                    }
                }
                return true;
            }

            inline nugget_type *insert( nugget_type *node ) throw()
            {
                assert( memory_is_ordered() );
                if(content.size<=0)
                {
                    content.push_back(node);
                    return node;
                }
                else
                {
                    if(node->data<content.head->data)
                    {
                        content.push_front(node);
                        return node;
                    }
                    else
                    {
                        if(node->data>content.tail->data)
                        {
                            content.push_back(node);
                            return node;
                        }
                        else
                        {
                            // generic case
                            assert(content.size>=2);
                            for( nugget_type *scan = content.head; scan->next; scan=scan->next )
                            {
                                assert(scan->data<scan->next->data);
                                if( (scan->data<node->data) && (node->data<scan->next->data) )
                                {
                                    content.insert_after(scan,node);
                                    assert( memory_is_ordered() );
                                    return node;
                                }
                            }
                            assert( die("nugget insertion failure") );
                            return 0;
                        }
                    }
                }


            }

            inline nugget_type *create_nugget()
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
                return insert(node);
            }
        };
    }
}


#endif

