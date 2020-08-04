

//! \file

#ifndef Y_MEMORY_SMALL_ZCACHE_INCLUDED
#define Y_MEMORY_SMALL_ZCACHE_INCLUDED 1

#include "y/core/pool.hpp"
#include "y/core/list.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            //__________________________________________________________________
            //
            //
            //! common ops for zcache
            //
            //__________________________________________________________________
            struct __zcache
            {
                static const size_t header = sizeof(void*);                     //!< reserved bytes for intenal linking
                static void * acquire(const size_t);                            //!< acquire(chunk_size)
                static void   release(void *,const size_t)            throw();  //!< acquire(chunk_size)
                static size_t length_from(const size_t, const size_t) throw();  //!< next_power_of_two(max_of(,))
                static void  *first(void *)                           throw();  //!< translated address
            };


            //__________________________________________________________________
            //
            //
            //! provide cache of zombie NODEs
            //
            //__________________________________________________________________
            template <typename NODE>
            class zcache
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________

                //! minimal increase in available nodes per allocation
                static const size_t min_nodes_rise = 2;
                //! mimimal bytes to allocate
                static const size_t min_chunk_size = __zcache::header + min_nodes_rise * sizeof(NODE);

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup from usr_chunk_size
                inline zcache(const size_t usr_chunk_size) throw() :
                chunk_size( __zcache::length_from(usr_chunk_size,min_chunk_size) ),
                nodes_rise( (chunk_size-__zcache::header)/sizeof(NODE) )
                {

                }

                //! cleanup
                inline ~zcache() throw()
                {
                    if( nodes_rise * parts.size != nodes.size )
                    {
                        // missing nodes
                    }
                    nodes.reset();
                    parts.reverse();
                    while(parts.size) __zcache::release(parts.query(),chunk_size);
                }

                //! return a zeroed NODE
                inline NODE *query_nil()
                {
                    if(nodes.size)
                    {
                        NODE *node = nodes.pop_front();
                        bzset(*node);
                        return node;
                    }
                    else
                    {
                        void *addr = __zcache::acquire(chunk_size);
                        (void) parts.store(static_cast<part *>(addr));
                        NODE *node = static_cast<NODE *>( __zcache::first(addr) );
                        assert( is_zeroed(*node) );
                        for(size_t i=1;i<nodes_rise;++i)
                        {
                            assert( is_zeroed(node[i]) );
                            nodes.push_back( &node[i] );
                        }
                        return node;
                    }
                }

                //! store a zombie node
                inline void store_nil(NODE *node) throw()
                {
                    assert(node);
                    assert(0==node->prev);
                    assert(0==node->next);
                    assert(owns(node));
                    nodes.push_front(node);
                }

                //! check ownership
                inline bool owns(const NODE *node) const throw()
                {
                    for(const part *p=parts.head;p;p=p->next)
                    {
                        if( owned_by(p,node) ) return true;
                    }
                    return false;
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________

                const size_t chunk_size; //!< new bytes per allocation
                const size_t nodes_rise; //!< new nodes per allocation


            private:
                Y_DISABLE_COPY_AND_ASSIGN(zcache);
                struct part
                {
                    part *next;
                };
                core::list_of<NODE> nodes;
                core::pool_of<part> parts;

                inline bool owned_by(const part *p, const NODE *n) const throw()
                {
                    assert(p); assert(n);
                    const NODE *base = static_cast<const NODE*>( __zcache::first( (void*)p ) );
                    const NODE *last = base + nodes_rise;
                    return n>=base&&n<last;
                }



            };

        }

    }

}

#endif
