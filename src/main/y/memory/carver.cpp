#include "y/memory/carver.hpp"
#include "y/type/utils.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {

        size_t carver:: compute_chunk_size(const size_t user_chunk_size) throw()
        {
            size_t cs = max_of<size_t>(user_chunk_size,slice::small_size);
            cs        = max_of<size_t>(cs,sizeof(void*)+sizeof(slice)   );

            return next_power_of_two(cs);
        }

        carver:: carver( const size_t user_chunk_size ) throw() :
        chunk_size( compute_chunk_size(user_chunk_size) ),
        acquiring(0),
        slices(),
        cached(),
        pages(),
        bytes(0),
        slices_per_page( (chunk_size - sizeof(void*))/sizeof(slice) )
        {
            
        }

        carver:: ~carver() throw()
        {

            if(pages.size)
            {
                static global &hmem = global::location();
                while(slices.size>0)
                {
                    slice *s = slices.pop_back();
                    s->~slice();
                    hmem.__free(s->entry,io::delta(s->entry,s->guard));
                }
                while(pages.size)
                {
                    hmem.__free(pages.query(),chunk_size);
                }
                cached.reset();
            }

        }
        
    }
    
}

namespace upsylon
{
    namespace memory
    {

        void * carver:: acquire( size_t &n )
        {
            static global &hmem = global::instance();

            if(n<=0) return NULL;

            if( acquiring )
            {
                assert(slices.size>0);
                assert(slices.owns(acquiring));
                void *p = acquiring->acquire(n);
                if(p)
                {
                    // fully cached!
                    return p;
                }
                else
                {
                    // search..
                    slice *lo = acquiring->prev;
                    slice *up = acquiring->next;
                    while(lo&&up)
                    {
                        {
                            void *p = lo->acquire(n);
                            if(p) return p;
                            lo = lo->prev;
                        }

                        {
                            void *p = up->acquire(n);
                            if(p) return p;
                            up=up->next;
                        }
                    }

                    while(lo)
                    {
                        void *p = lo->acquire(n);
                        if(p) return p;
                        lo = lo->prev;
                    }

                    while(up)
                    {
                        void *p = up->acquire(n);
                        if(p) return p;
                        up=up->next;
                    }
                    // search failure
                }
            }

            // cache missed and not enoug memory => create a new slice
            if(cached.size<=0)
            {
                slice          *s = io::cast<slice>( pages.store( static_cast<page *>(hmem.__calloc(1,chunk_size) ) ), sizeof(void*));
                for(size_t i=0;i<slices_per_page;++i)
                {
                    (void)cached.store(s+i);
                }
                (size_t&)bytes += chunk_size;
            }

            // create data
            assert(cached.size>0);
            const size_t buflen = max_of(slice::bytes_to_hold(n),chunk_size);
            void        *buffer = hmem.__calloc(1,buflen); assert(buffer);
            (size_t&)bytes += buflen;
            
            // create slice
            slice  *s = new ( cached.query() ) slice(buffer,buflen); slices.push_back(s);
            acquiring = s;
            void   *p = s->acquire(n);
            assert(p);
            return p;
        }

    }
}

namespace upsylon
{
    namespace memory
    {
        void carver:: release(void * &p, size_t &n) throw()
        {
            if(p)
            {
                assert(n>0);
                slice *s = slice::release(p,n);
                assert(slices.owns(s));
                if(s->is_empty())
                {

                }
            }
            else
            {
                assert(n<=0);
            }

        }

    }

}


