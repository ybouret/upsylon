//! \file
#ifndef Y_HASH_TABLE_INCLUDED
#define Y_HASH_TABLE_INCLUDED 1

#include "y/core/addr-list.hpp"
#include "y/memory/slab.hpp"
#include "y/type/utils.hpp"
#include "y/container/container.hpp"
#include "y/sort/merge.hpp"

namespace upsylon
{

    namespace core
    {
        //! base class for hash_table parameters
        class hash_table_info
        {
        public:
            inline virtual ~hash_table_info() throw() {}
            inline explicit hash_table_info() throw() {}
            static const size_t load_factor = 4; //!< maximum number of nodes per slot
            static const size_t min_slots   = 8; //!< minimum number of slots

        private:
            Y_DISABLE_COPY_AND_ASSIGN(hash_table_info);
        };

        //! hash table of NODE
        /**
         NODE must have :
         - a size_t hkey
         - a void *meta field
         - a const_key & key() const throw() function
         */
        template <typename NODE,typename ALLOCATOR = memory::global>
        class hash_table : public hash_table_info
        {
        public:
            typedef addr_node<NODE>            meta_node; //!< to store node addresses
            typedef addr_list<NODE>            meta_list; //!< list of node addresses
            typedef core::list_of<NODE>        slot_type; //!< a slot of the hash table
            typedef memory::slab_of<NODE>      node_slab; //!< to acquire/release node
            typedef memory::slab_of<meta_node> meta_slab; //!< to acquire/release meta

            //! default initialization
#define Y_CORE_HASH_TABLE_CTOR() hash_table_info(), chain(),slot(0),slots(0),smask(0),items(0),nodes(0,0),metas(0,0),buffer(0),allocated(0)
            //! empty table
            inline  hash_table() throw() : Y_CORE_HASH_TABLE_CTOR()
            {
            }

            //! self content n objects
            inline  hash_table(const size_t n) throw() :  Y_CORE_HASH_TABLE_CTOR()
            {
                setup(n);
            }

            //! hard copy another table
            inline hash_table( const hash_table &other ) :  Y_CORE_HASH_TABLE_CTOR()
            {
                setup(other.chain.size);
                duplicate(other);
            }



            //! uses NODE destructor to free memory
            inline void free() throw()
            {
                for(size_t i=0;i<slots;++i)
                {
                    slot_type &s = slot[i];
                    while(s.size)
                    {
                        NODE *node = s.pop_back();
                        __free(node);
                    }
                }
                assert( nodes.is_filled() );
                assert( metas.is_filled() );
            }

            //! release resources and store back node and meta
            inline void __free( NODE *node ) throw()
            {
                assert(node);
                assert(node->meta);
                metas.release( chain.unlink( static_cast<meta_node *>(node->meta) ) );
                node->~NODE();
                nodes.release( node );
            }
            

            //! clear all
            inline  virtual ~hash_table() throw()
            {
                if(allocated)
                {
                    static memory::allocator &hmem = ALLOCATOR::location();
                    free();
                    hmem.release(buffer,allocated);
                }
            }

            //! no throw swap
            inline void swap_with( hash_table &other ) throw()
            {
                upsylon::bswap(chain,other.chain);
                upsylon::cswap(slot,other.slot);
                upsylon::cswap(slots,other.slots);
                upsylon::cswap(smask,other.smask);
                upsylon::cswap(items,other.items);
                nodes.swap_with(other.nodes);
                metas.swap_with(other.metas);
                upsylon::cswap(buffer,other.buffer);
                upsylon::cswap(allocated,other.allocated);

            }

            //! creation/insertion of a new node with enough space
            template <
            typename KEY,
            typename T>
            bool insert(typename type_traits<KEY>::parameter_type node_key,
                        const size_t                              node_hkey,
                        typename type_traits<T>::parameter_type   node_data)
            {
                slot_type *pSlot = 0;
                if( search_node<KEY>(node_key,node_hkey,pSlot) )
                {
                    //__________________________________________________________
                    //
                    // multiple data
                    //__________________________________________________________
                    assert(pSlot);
                    return false; // multiple data
                }
                else
                {
                    NODE *node = 0;
                    if(chain.size>=items)
                    {
                        //______________________________________________________
                        //
                        // need to grow
                        //______________________________________________________
                        hash_table temp( container::next_capacity(items) );
                        temp.duplicate(*this);
                        node = temp.create<KEY,T>(node_key,node_hkey,node_data);
                        swap_with(temp);
                        pSlot = &slot[node_hkey&smask];
                    }
                    else
                    {
                        // slot already computed
                        assert(chain.size<items);
                        assert(pSlot);
                        node = this->create<KEY,T>(node_key,node_hkey,node_data);
                    }
                    assert(slot);
                    assert(pSlot);
                    assert(pSlot>=slot);
                    assert(pSlot<slot+slots);
                    pSlot->push_front(node);
                    return true;
                }

            }

            //! create a node and its chained meta node, need to be inserted
            template <
            typename KEY,
            typename T>
            NODE *create(typename type_traits<KEY>::parameter_type node_key,
                         const size_t                              node_hkey,
                         typename type_traits<T>::parameter_type   node_data)
            {
                assert(chain.size<items);
                NODE *node = nodes.acquire();
                try
                {
                    new (node) NODE(node_key,node_hkey,node_data);
                }
                catch(...)
                {
                    nodes.release(node);
                    throw;
                }
                assert(node_hkey==node->hkey);
                return start(node);
            }

            //! duplicate another table in this EMPTY table, using the NODE copy
            inline void duplicate(const hash_table &other)
            {
                assert(0==chain.size);
                assert(items>=other.chain.size);
                for(const meta_node *meta=other.chain.head;meta;meta=meta->next)
                {
                    assert(meta->addr);
                    const NODE &source = *(meta->addr);
                    NODE *node = nodes.acquire();
                    try
                    {
                        new (node) NODE(source);
                    }
                    catch(...)
                    {
                        nodes.release(node);
                        throw;
                    }
                    assert(source.hkey==node->hkey);
                    slot[node->hkey&smask].push_front( start(node) );
                }
            }

            //! search for a node with a given key/hash
            template <typename KEY>
            const NODE *search_node(typename type_traits<KEY>::parameter_type k,
                                    const size_t                              h,
                                    slot_type                               * &pSlot ) const throw()
            {
                assert(0==pSlot);
                if(0!=slot)
                {
                    slot_type &s = slot[h&smask];
                    pSlot        = &s;
                    for(const NODE *node=s.head;node!=0;node=node->next)
                    {
                        if( (h==node->hkey) && (k==node->key()) )
                        {
                            return node;
                        }
                    }
                    return 0;
                }
                else
                {
                    // no data at all
                    return 0;
                }
            }

            meta_list  chain; //!< list of meta nodes
            slot_type *slot;  //!< slots entry
            size_t     slots; //!< 0 or a power of two
            size_t     smask; //!< slots-1
            size_t     items; //!< max number of items
            node_slab  nodes; //!< to store nodes
            meta_slab  metas; //!< to store metas node


            //! sort internal chain by data
            template <typename COMPARE_DATA>
            inline void sort_by_data( COMPARE_DATA &compare_data )
            {
                merging<meta_node>::sort(chain,__compare_node_data<COMPARE_DATA>, (void *)&compare_data);
            }

            //! sort internal chain by keys
            template <typename COMPARE_KEYS>
            inline void sort_by_keys( COMPARE_KEYS &compare_keys )
            {
                merging<meta_node>::sort(chain,__compare_node_keys<COMPARE_KEYS>, (void*)&compare_keys);
            }

            //! allocated
            inline size_t allocated_bytes() const throw() { return allocated; }
            
        private:
            Y_DISABLE_ASSIGN(hash_table);
            void   *buffer;
            size_t  allocated;

            inline NODE * start(NODE *node) throw()
            {
                assert(0==node->meta);
                meta_node *meta = metas.acquire();
                new (meta) meta_node(node);
                node->meta = meta;
                chain.push_back(meta);
                return node;
            }

            template <typename COMPARE_DATA> static inline
            int __compare_node_data( const meta_node *lhs, const meta_node *rhs, void *args )
            {
                assert(args); assert(lhs); assert(rhs);
                COMPARE_DATA &compare_data = *(COMPARE_DATA *)args;
                return compare_data(lhs->addr->data,rhs->addr->data);
            }

            template <typename COMPARE_KEYS> static inline
            int __compare_node_keys( const meta_node *lhs, const meta_node *rhs, void *args )
            {
                assert(args); assert(lhs); assert(rhs);
                COMPARE_KEYS &compare_keys = *(COMPARE_KEYS *)args;
                return compare_keys(lhs->addr->key(),rhs->addr->key());
            }

            inline void setup(const size_t n)
            {
                if(n>0)
                {
                    static memory::allocator &hmem = ALLOCATOR::instance();

                    const size_t num_slots    = next_power_of_two(max_of<size_t>(n/load_factor,min_slots));
                    const size_t slots_offset = 0;
                    const size_t slots_length = num_slots * sizeof(slot_type);
                    const size_t nodes_offset = memory::align(slots_offset+slots_length);
                    const size_t nodes_length = node_slab::bytes_for(n);
                    const size_t metas_offset = memory::align(nodes_offset+nodes_length);
                    const size_t metas_length = meta_slab::bytes_for(n);

                    allocated = memory::align(metas_offset+metas_length);
                    char *p   = static_cast<char *>( buffer    = hmem.acquire(allocated) );
                    slot      = static_cast<slot_type *>( (void *) &p[slots_offset] );
                    new ( &nodes ) node_slab( &p[nodes_offset], nodes_length );
                    new ( &metas ) meta_slab( &p[metas_offset], metas_length );
                    slots = num_slots;
                    smask = num_slots-1;
                    items = n;
                    assert(nodes.capacity()>=n);
                    assert(metas.capacity()>=n);
                }
            }
        };
    }

}

#endif

