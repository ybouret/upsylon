//! \file
#ifndef Y_HASH_TABLE_INCLUDED
#define Y_HASH_TABLE_INCLUDED 1

#include "y/core/addr-list.hpp"
#include "y/memory/slab.hpp"
#include "y/type/utils.hpp"
#include "y/container/container.hpp"

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
            typedef addr_node<NODE>         meta_node; //!< to store node addresses
            typedef addr_list<NODE>         meta_list; //!< list of node addresses
            typedef core::list_of<NODE>     slot_type; //!< a slot of the hash table
            typedef memory::slab<NODE>      node_slab; //!< to acquire/release node
            typedef memory::slab<meta_node> meta_slab; //!< to acquire/release meta

            //! default initialization
#define Y_CORE_HASH_TABLE_CTOR() chain(),slot(0),slots(0),smask(0),nodes(0,0),metas(0,0),buffer(0),allocated(0)
            //! empty table
            inline  hash_table() throw() : Y_CORE_HASH_TABLE_CTOR()
            {
            }

            //! self content n objects
            inline  hash_table(const size_t n) throw() : Y_CORE_HASH_TABLE_CTOR()
            {
                std::cerr << "hash_table(" << n << ")" << std::endl;
                if(n>0)
                {
                    static memory::allocator &hmem = ALLOCATOR::instance();

                    const size_t num_slots  = next_power_of_two(max_of<size_t>(n/load_factor,min_slots));
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
                    std::cerr << "|_#slots=" << slots << std::endl;
                }
            }

            //! uses NODE destructor to free memory
            inline void free() throw()
            {
                std::cerr << "[hash_table.free] scanning #slots=" << slots << std::endl;
                for(size_t i=0;i<slots;++i)
                {
                    slot_type &s = slot[i];
                    while(s.size)
                    {
                        NODE *node = s.pop_back();
                        assert(node->meta);
                        metas.release( chain.unlink( static_cast<meta_node *>(node->meta) ) );
                        node->~NODE();
                        nodes.release( node );
                    }
                }
                assert( nodes.is_filled() );
                assert( metas.is_filled() );
            }

            

            //! clear all
            inline  virtual ~hash_table() throw()
            {
                if(allocated)
                {
                    std::cerr << "[hash_table.kill] #allocated=" << allocated << std::endl;
                    static memory::allocator &hmem = ALLOCATOR::location();
                    free();
                    hmem.release(buffer,allocated);
                }
            }

            //! no throw swap
            inline void swap_with( hash_table &other ) throw()
            {
                upsylon::bswap(chain,other.chain);
                upsylon::bswap(slot,other.slot);
                upsylon::bswap(slots,other.slots);
                upsylon::bswap(smask,other.smask);
                upsylon::bswap(items,other.items);
                upsylon::bswap(nodes,other.nodes);
                upsylon::bswap(metas,other.metas);
                upsylon::bswap(buffer,other.buffer);
                upsylon::bswap(allocated,other.allocated);

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
                if( search_node<KEY>(node_key,node_hkey,&pSlot) )
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
                        const size_t nxt_capa = container::next_capacity(items);
                        std::cerr << "[hash_table.grow] " << items << " -> " << nxt_capa << std::endl;
                        hash_table temp( nxt_capa );
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

            //! search for a node with a given key/hahs
            template <typename KEY>
            const NODE *search_node(typename type_traits<KEY>::parameter_type k,
                                    const size_t                              h,
                                    slot_type                               **ppSlot ) const throw()
            {
                assert(ppSlot);
                assert(! *ppSlot);
                if(slot)
                {
                    slot_type &s = * (*ppSlot= &slot[h&smask]);
                    for(const NODE *node=s.head;node;node=node->next)
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


        private:
            Y_DISABLE_COPY_AND_ASSIGN(hash_table);
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
        };
    }

}

#endif

