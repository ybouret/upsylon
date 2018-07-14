//! \file
#ifndef Y_HASH_TABLE_INCLUDED
#define Y_HASH_TABLE_INCLUDED 1

#include "y/core/addr-list.hpp"
#include "y/memory/slab.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{

    namespace core
    {
        class hash_table_info
        {
        public:
            inline virtual ~hash_table_info() throw() {}
            inline explicit hash_table_info() throw() {}
            static const size_t load_factor = 4;
            static const size_t min_slots   = 8;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(hash_table_info);
        };

        //! hash table of NODE
        /**
         NODE must have :
         - a size_t hkey
         - a void *meta field
         */
        template <typename NODE,typename ALLOCATOR = memory::global>
        class hash_table : public hash_table_info
        {
        public:
            typedef addr_node<NODE>         meta_node;
            typedef addr_list<NODE>         meta_list;
            typedef core::list_of<NODE>     slot_type;
            typedef memory::slab<NODE>      node_slab;
            typedef memory::slab<meta_node> meta_slab;

            //! default initialization
#define Y_CORE_HASH_TABLE_CTOR() chain(),slot(0),slots(0),smask(0),nodes(0,0),metas(0,0),buffer(0),allocated(0)
            //! empty table
            inline  hash_table() throw() : Y_CORE_HASH_TABLE_CTOR()
            {
            }

            //! self content n objects
            inline  hash_table(const size_t n) throw() : Y_CORE_HASH_TABLE_CTOR()
            {
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
                }
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

            template <
            typename KEY,
            typename T>
            void insert(typename type_traits<KEY>::parameter_type node_key,
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
                hook(node);
            }

            //! duplicate
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
                    hook(node);
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

            inline  void hook( NODE *node ) throw()
            {
                slot[ node->hkey & smask].push_front(node);
                assert(0==node->meta);
                meta_node *meta = metas.acquire();
                new (meta) meta_node(node);
                node->meta = meta;
                chain.push_back(meta);
            }
        };
    }

}

#endif

