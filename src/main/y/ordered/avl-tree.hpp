#ifndef Y_AVL_TREE_INCLUDED
#define Y_AVL_TREE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/comparator.hpp"
#include "y/object.hpp"

namespace upsylon
{

    struct avl
    {
        static const int at_left  = -1;
        static const int balanced =  0;
        static const int at_right =  1;

        template <typename T>
        class node
        {
        public:
            Y_DECL_ARGS(T,type);
            node      *parent;
            node      *left;
            node      *right;
            int        weight;
            const_type data;

            inline node(param_type user_data) : parent(0), left(0), right(0), weight(balanced), data(user_data) {}
            inline ~node() throw() {  }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node);
        };


        template <
        typename T,
        typename COMPARATOR = increasing_comparator<T>
        >
        class tree : public container
        {
        public:
            typedef node<T> node_type;

            inline explicit tree() throw() : root(0), nodes(0), pool_node(0), pool_size(0)
            {
            }

            inline virtual ~tree() throw()
            {
                delete_pool();
            }

            inline virtual size_t size()     const throw() { return nodes; }
            inline virtual size_t capacity() const throw() { return nodes+pool_size; }
            inline virtual void   free()     throw()       { free__(root); }
            inline virtual void   release()  throw()       { release__();  }

            inline virtual void reserve(const size_t n)
            {
                for(size_t i=n;i>0;--i) store( new_node() );
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(tree);
            node_type *root;
            size_t     nodes;

            node_type *pool_node;
            size_t     pool_size;

            inline void free__(node_type * &node) throw()
            {
                if(node)
                {
                    free__(node->left);
                    free__(node->right);
                    destruct(node);
                    node->parent = 0;
                    store(node);
                    node = 0;
                }
            }

            inline void kill__(node_type * &node) throw()
            {
                if(node)
                {
                    free__(node->left);
                    free__(node->right);
                    destruct(node);
                    object::release1(node);
                }
            }


            inline void release__() throw()
            {
                kill__(root);
                delete_pool();
            }

            inline static node_type *new_node() { return object::acquire1<node_type>(); }

            inline void       store( node_type *node ) throw()
            {
                assert(node);
                assert(0==node->parent);
                node->parent = pool_node;
                pool_node    = node;
                ++pool_size;
            }

            inline void delete_pool() throw()
            {
                while(pool_node)
                {
                    assert(pool_size>0);
                    node_type *curr = pool_node;
                    pool_node       = curr->parent;
                    object::release1(curr);
                    --pool_size;
                }
            }

        };

    };
}

#endif
