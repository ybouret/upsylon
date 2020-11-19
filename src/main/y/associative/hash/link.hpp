

//! \file
#ifndef Y_HASHED_LINK_INCLUDED
#define Y_HASHED_LINK_INCLUDED 1

#include "y/associative/hash/slots.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"

namespace upsylon
{

    //! interface type
    /**
     NODE derived from 'object' and has:
     - a 'hash_meta_node *meta' field
     - a 'const_key_type &key() const throw() ' field
     */
    template <typename KEY, typename NODE>
    class hash_link
    {
    public:
        Y_DECL_ARGS(KEY,key);

        typedef core::list_of_cpp<NODE> list_type;
        typedef core::list_of<NODE>     pool_type;

        list_type      chain;
        hash_slots     slots;
        hash_meta_pool metas; //!< pool of meta nodes
        pool_type      nodes; //!< pool of NODEs

        inline explicit hash_link(const size_t num_slots) :
        chain(), slots(num_slots), metas(), nodes()
        {
        }

        inline virtual ~hash_link() throw()
        {

        }

        inline void trim() throw()
        {
            assert(nodes.size==metas.size);
            metas.erase();
            while(nodes.size)
            {
                NODE *node = nodes.pop_back();
                object::release1(node);
            }
        }

        void reserve_pair()
        {
            nodes.push_back( object::acquire1<NODE>() );
        }

        inline NODE *search_at(const size_t       hkey,
                               hash_meta_list *  &slot,
                               const_key         &key) throw()
        {
            assert(0==slot);
            for(hash_meta_node *meta = (slot=&slots[hkey])->head;meta;meta=meta->next)
            {
                NODE *node = static_cast<NODE *>(meta->addr); assert(node);
                if( key == node->key() )
                {
                    slot->move_to_front(meta);
                    return node;
                }
            }
            return NULL;
        }

        inline void insert_at(const size_t    hkey,
                              hash_meta_list *slot,
                              NODE           *node)
        {
            try
            {
                assert(node);
                assert(slot);
                slot->push_front(node->meta=metas.query(hkey,node));
            }
            catch(...)
            {
                // memory failure;
                self_destruct(*node);
                nodes.push_front(node);
                throw;
            }
        }




    private:
        Y_DISABLE_COPY_AND_ASSIGN(hash_link);
    };

}

#endif
