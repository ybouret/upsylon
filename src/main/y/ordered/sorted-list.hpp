//! \file
#ifndef Y_SORTED_LIST_INCLUDED
#define Y_SORTED_LIST_INCLUDED 1

#include "y/container/ordered.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{

    template <typename T>
    class sorted_list // : public ordered<T>
    {
    public:
        Y_DECL_ARGS(T,type);
        class node_type
        {
        public:
            node_type   *next;
            node_type   *prev;
            mutable_type data;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
            node_type(); ~node_type() throw();
        };

        typedef core::list_of<node_type> list_type;
        typedef core::pool_of<node_type> pool_type;


        inline virtual ~sorted_list() throw() { release__(); }

        inline explicit sorted_list() throw() : content(), dormant() {}

        inline virtual size_t size()     const throw() { return content.size; }
        inline virtual size_t capacity() const throw() { return content.size + dormant.size; }

        inline virtual void free()    throw() { free__(); }
        inline virtual void release() throw() { release__(); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(sorted_list);
        list_type  content;
        pool_type  dormant;

        node_type *query( param_type args )
        {
            node_type *node = (dormant.size>0) ? dormant.query() : object::acquire1<node_type>();
            try
            {
                new (&(node->data)) mutable_type( args );
            }
            catch(...)
            {
                dormant.store(node);
                throw;
            }
            return node;
        }

        void put_to_sleep(node_type *node) throw()
        {
            assert(node); assert(NULL==node->next); assert(NULL==node->prev);
            self_destruct(dormant.store(node)->data);
        }

        inline void reserve__(size_t n)
        {
            while(n-->0)
            {
                dormant.store( object::acquire1<node_type>() );
            }
        }

        inline void free__() throw()
        {
            while( content.size ) put_to_sleep( content.pop_back() );
        }

        inline void release__() throw()
        {
            while( content.size ) { node_type *node = content.pop_back(); self_destruct(node->data); object::release1(node); }
            while( dormant.size ) { node_type *node = dormant.query();                               object::release1(node); }
        }


    };

}

#endif

