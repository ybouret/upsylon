#include "y/memory/carver.hpp"
#include "y/memory/global.hpp"
#include "y/memory/arena-of.hpp"
#include "y/type/utils.hpp"
#include "y/exception.hpp"

#include <iostream>
#include <cstring>

namespace upsylon
{
    namespace memory
    {

        size_t carver:: compute_chunk_size(const size_t user_chunk_size) throw()
        {
            size_t cs = Y_MEMALIGN(user_chunk_size);
            if(cs<=slice::small_size) cs = slice::small_size;
            return next_power_of_two(cs);
        }

        namespace
        {
            typedef arena_of<slice> arena_type;
        }

        void carver:: clr() throw()
        {
            memset(wksp,0,sizeof(wksp));
        }

        carver:: carver( const size_t user_chunk_size ) throw() :
        chunk_size( compute_chunk_size(user_chunk_size) ),
        acquiring(0),
        slices(),
        wksp(),
        impl( &wksp[0] )
        {
            assert( sizeof(wksp) >= sizeof( arena_of<slice> ) );
            clr();
            new (impl) arena_type(chunk_size);
        }

        carver:: ~carver() throw()
        {
            assert(impl);
            arena_type &a = *static_cast<arena_type *>(impl);
            while(slices.size>0)
            {
                static global &hmem = global::location();
                slice *s = slices.pop_back();
                s->~slice();
                hmem.__free(s->entry,io::delta(s->entry,s->guard));
                a.release(s);
            }
            destruct( &a );
            clr();
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
                    //__________________________________________________________
                    //
                    // fully cached!
                    //__________________________________________________________
                    return p;
                }
                else
                {
                    //__________________________________________________________
                    //
                    // interleaved search
                    //__________________________________________________________
                    slice *lo = acquiring->prev;
                    slice *up = acquiring->next;
                    while(lo&&up)
                    {
                        {
                            void *p = lo->acquire(n);
                            if(p)
                            {
                                acquiring = lo;
                                return p;
                            }
                            lo = lo->prev;
                        }

                        {
                            void *p = up->acquire(n);
                            if(p)
                            {
                                acquiring = up;
                                return p;

                            }
                            up=up->next;
                        }
                    }

                    //__________________________________________________________
                    //
                    // down search
                    //__________________________________________________________
                    while(lo)
                    {
                        void *p = lo->acquire(n);
                        if(p)
                        {
                            acquiring = lo;
                            return p;
                        }
                        lo = lo->prev;
                    }

                    //__________________________________________________________
                    //
                    // up search
                    //__________________________________________________________
                    while(up)
                    {
                        void *p = up->acquire(n);
                        if(p)
                        {
                            acquiring = up;
                            return p;
                        }
                        up=up->next;
                    }
                    //__________________________________________________________
                    //
                    // search failure
                    //__________________________________________________________
                }
            }

            //__________________________________________________________________
            //
            // cache missed and not enough memory:
            // => get a new block
            //__________________________________________________________________
            arena_type  &a      = *static_cast<arena_type *>(impl);
            slice       *s      = a.acquire();
            const size_t buflen = max_of(slice::bytes_to_hold(n),chunk_size);
            void        *buffer = 0;

            try
            {
                //______________________________________________________________
                //
                // => memory for this slice
                //______________________________________________________________
                buffer = hmem.__calloc(1,buflen); assert(buffer);
            }
            catch(...)
            {
                a.release(s);
                throw;
            }
            //__________________________________________________________________
            //
            // update and get memory
            //__________________________________________________________________
            assert(buffer);
            acquiring = slices.push_back( new (s) slice(buffer,buflen) );
            void   *p = s->acquire(n);
            assert(p);
            while( s->prev && s->entry<s->prev->entry)
            {
                slices.towards_head(s);
            }
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
                    //std::cerr << "Empty Slice!" << std::endl;
                }
            }
            else
            {
                assert(n<=0);
            }

        }

    }

}

namespace upsylon
{
    namespace memory
    {
        bool carver:: defragment( void * &addr, size_t &capa, const size_t size) throw()
        {
            assert(addr);
            assert(capa);
            slice *curr = slice::owner_of(addr); assert(slices.owns(curr));

            for( slice *scan=slices.head; scan != curr; scan=scan->next)
            {
                assert(scan->entry<curr->entry);
                slice *stmp = scan->receive(addr,capa,size);
                if(stmp)
                {
                    assert(stmp==curr);
                    return true;
                }
            }


            return false;
        }

    }

}



