//! \file
#ifndef Y_MEMORY_NUGGETS_INCLUDED
#define Y_MEMORY_NUGGETS_INCLUDED 1

#include "y/memory/nugget.hpp"
#include "y/memory/arena.hpp"
#include "y/object-parameters.hpp"
#include "y/os/error.hpp"

namespace upsylon
{
    namespace memory
    {
        //! base class for common interface
        class nuggets
        {
        public:
            static const size_t small_chunk_size = Y_CHUNK_SIZE; //!< base size for allocation
            static const size_t min_num_blocks   = 1<<3;         //!< minimum number of blocks per nugget

            //! destructor
            virtual ~nuggets() throw();

            //! common acquire method
            virtual void *acquire() = 0;
            //! common release method
            virtual void  release(void *p) throw() = 0;

            //! the total number of blocks
            virtual size_t get_num_blocks() const throw() = 0;
            //! the bits per block
            virtual size_t get_block_bits() const throw() = 0;
            //! the bytes per block
            virtual size_t get_block_size() const throw() = 0;
            //! the big chunk size
            virtual size_t get_chunk_size() const throw() = 0;

            static void * global_calloc( const size_t count, const size_t size);
            static void   global_free(void *p, const size_t bytes) throw();

        protected:
            //! constructor
            explicit nuggets() throw();

            //! page to store individual nugget memory
            struct page
            {
                page *next; //!< for pool
            };
        private:
            Y_DISABLE_COPY_AND_ASSIGN(nuggets);
        };

        //! multiple nuggets of same block_size
        template <const size_t BLOCK_BITS>
        class nuggets_for : public nuggets
        {
        public:
            typedef nugget<BLOCK_BITS> nugget_type; //!< the one nugget
            static  const size_t       nuggets_per_page = (small_chunk_size - sizeof(void*))/sizeof(nugget_type); //!< ever growing pages
            static  const size_t       block_size = nugget_type::block_size; //!< the nugget block size

            //! number of blocks per nugget, simple heuristic
            static inline
            size_t compute_num_blocks()
            {
                assert(is_a_power_of_two(small_chunk_size));
                assert(is_a_power_of_two(block_size));
                const size_t min_chunk_size = block_size * min_num_blocks;
                if(min_chunk_size<=small_chunk_size)
                {
                    const size_t nb = small_chunk_size/block_size;
                    assert(is_a_power_of_two(nb));
                    return nb;
                }
                else
                {
                    size_t chunk_size = small_chunk_size;
                    while(chunk_size<min_chunk_size)
                    {
                        const size_t next_chunk_size = chunk_size << 1;
                        // todo: check
                        chunk_size = next_chunk_size;
                    }
                    assert(is_a_power_of_two(chunk_size));
                    const size_t nb = chunk_size/block_size;
                    assert(is_a_power_of_two(nb));
                    return nb;
                }
            }

            //! compute memory characteristics \todo work on that
            inline explicit nuggets_for() :
            nuggets(),
            acquiring(0),
            releasing(0),
            available(0),
            content(),
            cached(),
            pages(),
            num_blocks( compute_num_blocks()    ),
            chunk_size( num_blocks * block_size )
            {
            }

            inline virtual size_t get_block_bits() const throw() { return nugget_type::block_bits; }
            inline virtual size_t get_block_size() const throw() { return nugget_type::block_size; }
            inline virtual size_t get_num_blocks() const throw() { return num_blocks; }
            inline virtual size_t get_chunk_size() const throw() { return num_blocks; }


            //! release all memory
            inline virtual ~nuggets_for() throw()
            {
                if(pages.size)
                {
                    for(nugget_type *node=content.tail;node;node=node->prev)
                    {
                        nuggets::global_free(node->data,node->bytes);
                    }
                    content.reset();
                    cached.reset();
                    while(pages.size)
                    {
                        nuggets::global_free(pages.query(),small_chunk_size);
                    }
                }
            }

            //! acquire one block of block_size bytes
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
                        fatal_error("[nuggets.acquire] invalid memory");
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

                fatal_error("[nuggets.release] invalid memory");

            RELEASE:
                assert(releasing);
                assert(releasing->owns(p));
                releasing->release(p);
                ++available;
            }

        private:
            nugget_type               *acquiring;
            nugget_type               *releasing;
            size_t                     available;
            core::list_of<nugget_type> content;
            core::pool_of<nugget_type> cached;
            core::pool_of<page>        pages;
            const size_t               num_blocks;
            const size_t               chunk_size;

            Y_DISABLE_COPY_AND_ASSIGN(nuggets_for);
            
            inline void create_nuggets()
            {
                assert(cached.size<=0);
                nugget_type *node = io::cast<nugget_type>(pages.store(static_cast<page *>(nuggets::global_calloc(1,small_chunk_size))),sizeof(void*));
                for(size_t i=0;i<nuggets_per_page;++i)
                {
                    (void)cached.store( node+i );
                }
            }

            //! debug only
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

            //! insert a node at its place
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

                            // should never get here
                            delete node;
                            fatal_error("[nuggets.insert] invalid memory");
                            return 0;
                        }
                    }
                }


            }

            //! create a new nugget with full memory handling
            inline nugget_type *create_nugget()
            {
                if(cached.size<=0)
                {
                    create_nuggets();
                }
                assert(cached.size>0);
                void        *data = nuggets::global_calloc(1,chunk_size);
                nugget_type *node = new ( cached.query() ) nugget_type(chunk_size,data);
                assert(num_blocks==node->still_available);
                available += num_blocks;
                return insert(node);
            }
        };
    }
}


#endif

