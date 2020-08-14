//! \file

#ifndef Y_MEMORY_TIGHT_ZCACHE_INCLUDED
#define Y_MEMORY_TIGHT_ZCACHE_INCLUDED 1

#include "y/memory/tight/quarry.hpp"
#include "y/core/pool.hpp"
#include "y/type/block/zset.hpp"
#include "y/sort/merge.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {


            //__________________________________________________________________
            //
            //
            //! common ops for zcache
            //
            //__________________________________________________________________
            struct __zcache
            {
                // types and definitions
                struct part { part  *next; };                                       //!< binary layout for part
                static const size_t header = sizeof(part);                          //!< reserved bytes for intenal linking

                // helpers
                static size_t chunk_size_from(const size_t, const size_t) throw();  //!< next_power_of_two(max_of(,,stones::min_size))
                static void  *first_node_of(void *)                       throw();  //!< translated address
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
                typedef __zcache::part      part;       //!< alias
                typedef core::pool_of<part> parts_type; //!< alias
                typedef core::list_of<NODE> nodes_type; //!< alias

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
                 =      |header|<-          nodes              ->|nope | (chunk_size)
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
                        NODE *node = static_cast<NODE *>( __zcache::first_node_of(addr) );
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



                void gc() throw()
                {
                    //----------------------------------------------------------
                    // sort nodes, lowest memory first
                    //----------------------------------------------------------

                    merging<NODE>::sort_by_increasing_address(nodes);

                    //----------------------------------------------------------
                    // explore all parts, using a local stack
                    //----------------------------------------------------------
                    parts_type  stk;
                    while(parts.size>0)
                    {
                        part *probe = parts.query();
                        NODE *lower = lower_node_of(probe);
                        if(nodes.owns(lower))
                        {
                            NODE  *node  = lower->next;
                            for(size_t i=1;i<nodes_rise;++i,node=node->next)
                            {
                                if(lower+i!=node)
                                {
                                    goto KEEP_PAGE;
                                }
                            }
                            assert(nodes.owns(upper_node_of(probe)));

                            //--------------------------------------------------
                            // cleanup
                            //--------------------------------------------------
                            for(size_t i=nodes_rise;i>0;--i)
                            {
                                (void) nodes.unlink( lower++ );
                            }
                            memIO.release(probe);
                            continue;
                        }


                    KEEP_PAGE:
                        stk.store(probe);
                    }

                    //----------------------------------------------------------
                    // restore the current parts from remaining stack
                    //----------------------------------------------------------
                    while(stk.size>0)
                    {
                        parts.store( stk.query() );
                    }

                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t chunk_size; //!< new bytes per allocation           |+1=1
                const size_t nodes_rise; //!< new nodes per allocation           |+1=2
            private:
                Y_DISABLE_COPY_AND_ASSIGN(zcache);

                vein      &memIO;    //!< for alloc/free of parts with same size |+1=3
                nodes_type nodes;    //!< usable nodes                           |+4=7
                parts_type parts;    //!< holding all nodes                      |+3=10

                inline bool owned_by(const part *p, const NODE *n) const throw()
                {
                    assert(p); assert(n);
                    const NODE *base = lower_node_of(p);
                    const NODE *last = base + nodes_rise;
                    return n>=base&&n<last;
                }

                inline NODE *upper_node_of(const part *p) const throw()
                {
                    NODE *n = lower_node_of(p) + nodes_rise;
                    return --n;
                }

                inline NODE *lower_node_of(const part *p) const throw()
                {
                    return static_cast<NODE*>( __zcache::first_node_of( (void*)p ) );
                }

                
            };

        }

    }

}

#endif
