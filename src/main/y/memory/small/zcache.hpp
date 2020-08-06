//! \file

#ifndef Y_MEMORY_SMALL_ZCACHE_INCLUDED
#define Y_MEMORY_SMALL_ZCACHE_INCLUDED 1

#include "y/memory/small/quarry.hpp"
#include "y/core/pool.hpp"
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
                static const size_t header = sizeof(void*);                         //!< reserved bytes for intenal linking
                static size_t chunk_size_from(const size_t, const size_t) throw();  //!< next_power_of_two(max_of(,,stones::min_size))
                static void  *first(void *)                               throw();  //!< translated address
                static void   missing(const size_t,const unsigned)        throw();  //!< display missing node
            };


            //__________________________________________________________________
            //
            //
            //! provide cache of zombie NODEs
            /**
             zombie nodes are store in a list of nodes, when
             they are allocated by ever-growing parts of chunk_size,
             using a dedicated vein
             */
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

                //! minimal increase in nodes per allocation
                static const size_t min_nodes_rise = 2;
                //! mimimal bytes to allocate
                static const size_t min_chunk_size = __zcache::header + min_nodes_rise * sizeof(NODE);

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup from usr_chunk_size
                /**
                 usr_chunk_size is modified to hold enough data, and
                 the number of zombie nodes created per allocation is computed
                 */
                inline zcache(const size_t usr_chunk_size, quarry &Q) throw() :
                chunk_size( __zcache::chunk_size_from(usr_chunk_size,min_chunk_size) ),
                nodes_rise( (chunk_size-__zcache::header)/sizeof(NODE) ),
                memIO( Q(chunk_size) ),
                nodes(),
                parts()
                {

                }

                //! return all memory
                inline ~zcache() throw()
                {
                    // sanity check
                    const size_t allocated = nodes_rise * parts.size;
                    const size_t available = nodes.size; assert(allocated>=available);
                    if(available<allocated ) __zcache::missing(allocated-available,sizeof(NODE));

                    // cleanup
                    nodes.reset();                                    // dismiss zombies
                    while(parts.size) memIO.release( parts.query() ); // return parts
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! return a zeroed NODE
                /**
                 a memory area is formatted like this:
                 part = {part *next;...;node[0]..node[nodes_rise-1];...}
                 |header|<-          nodes              ->|nope | (chunk_size)
                 */
                inline NODE *zquery()
                {
                    if(nodes.size)
                    {
                        //------------------------------------------------------
                        // some nodes are available
                        //------------------------------------------------------
                        NODE *node = nodes.pop_front();
                        bzset(*node);
                        return node;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // get a (dirty) memory are of chunk_size
                        //------------------------------------------------------
                        void *addr = memIO.acquire();
                        {
                            part  *p = static_cast<part *>(addr);
                            bzset(*p);
                            (void) parts.store(p);
                        }
                        NODE *node = static_cast<NODE *>( __zcache::first(addr) );
                        bzset(*node);
                        for(size_t i=1;i<nodes_rise;++i)
                        {
                            NODE  *temp = &node[i];
                            bzset(*temp);
                            (void) nodes.push_back( temp );
                        }
                        return node;
                    }
                }

                //! store a zombie node
                inline void zstore(NODE *node) throw()
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
                struct part { part  *next; }; //!< binary layout for part
                vein               &memIO;    //!< for alloc/free of parts with same size
                core::list_of<NODE> nodes;    //!< usable nodes
                core::pool_of<part> parts;    //!< holding all nodes

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
