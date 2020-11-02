
//! \file
#ifndef Y_PREFIX_TREE_INCLUDED
#define Y_PREFIX_TREE_INCLUDED 1

#include "y/associative/prefix/stem.hpp"

namespace upsylon {

    template <typename CODE, typename T>
    class prefix_tree
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                             //!< aliases
        class   data_node;
        typedef prefix_stem<CODE,data_node>   stem_type; //!< alias
        typedef typename stem_type::node_type node_type; //!< alias

        class data_node
        {
        public:
            inline  data_node(const_type &args) : next(0), prev(0), hook(0), data(args) {}
            inline ~data_node() throw() { hook=0; }

            data_node *next;
            data_node *prev;
            node_type *hook;
            type       data;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_node);
        };
        typedef core::list_of<data_node> data_list;
        typedef core::pool_of<data_node> data_pool;


        inline explicit prefix_tree() : dl(), dp(), db()
        {
        }

        inline virtual ~prefix_tree() throw()
        {
        }

        template <typename KEY>
        bool insert(KEY        &key,
                    param_type &args)
        {
            data_node *node = make_node(args);
            try {
                node_type *mark = 0;
                if(!db.grow(key,node,&mark))
                {
                    return false;
                }
                else
                {
                    node->hook = mark;
                    return true;
                }
            }
            catch(...)
            {
                kill_node(node);
                throw;
            }
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_tree);
        data_list dl; //!< list of hooked, live data nodes
        data_pool dp; //!< pool of zombie data nodes
        stem_type db; //!< the database

        data_node *make_node(const_type &args)
        {
            data_node *node = dp.size ? dp.query() : object::acquire1<data_node>();
            try
            {
                return new (node) data_node(args);
            }
            catch(...)
            {
                dp.store(node);
                throw;
            }
        }

        void kill_node(data_node *node) throw()
        {
            assert(node); assert(0==node->next); assert(0==node->prev);
            self_destruct(*node);
            dp.store(node);
        }

    };

}

#endif
