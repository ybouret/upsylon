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
            Y_DECL_ARGS(T,type);       //!< alias
            typedef node<T> node_type; //!< alias

            inline explicit tree() throw() : root(0), nodes(0), pool_node(0), pool_size(0)
            {
            }

            inline virtual ~tree() throw()
            {
                release__();
            }

            inline virtual size_t size()     const throw() { return nodes; }
            inline virtual size_t capacity() const throw() { return nodes+pool_size; }
            inline virtual void   free()     throw()       { free__(root); }
            inline virtual void   release()  throw()       { release__();  }
            inline virtual void   reserve(const size_t n)  { for(size_t i=n;i>0;--i) store( new_node() ); }

            inline void insert(param_type data)
            {
                node_type *node = create_node_for(data); assert(NULL==node->parent);
                if(root)
                {
                    insert__(node,root);
                }
                else
                {
                    assert(0==nodes);
                    root = node;
                }
                ++nodes;
            }

            template <typename FUNCTION> inline void inorder(FUNCTION &F)   const { inorder<FUNCTION>(F,root,0);   }
            template <typename FUNCTION> inline void preorder(FUNCTION &F)  const { preorder<FUNCTION>(F,root,0);  }
            template <typename FUNCTION> inline void postorder(FUNCTION &F) const { postorder<FUNCTION>(F,root,0); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tree);
            node_type *root;
            size_t     nodes;

            node_type *pool_node;
            size_t     pool_size;

            mutable COMPARATOR compare;

            inline void insert__( node_type *node, node_type * curr )
            {
                assert(node);
                assert(curr);
                if(compare(node->data,curr->data)<=0)
                {
                    // should insert at left
                    if(0==curr->left)
                    {
                        curr->left  = node;
                        node->parent = curr;
                    }
                    else
                    {
                        insert__(node,curr->left);
                    }
                }
                else
                {
                    // should insert at right
                    if(0==curr->right)
                    {
                        curr->right = node;
                        node->parent = curr;
                    }
                    else
                    {
                        insert__(node,curr->right);
                    }
                }
            }

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

            inline node_type *query()
            {
                if(pool_node)
                {
                    assert(pool_size>0);
                    node_type *node = pool_node;
                    pool_node       = node->parent;
                    node->parent    = 0;
                    --pool_size;
                    return node;
                }
                else
                {
                    return new_node();
                }
            }

            inline node_type *create_node_for( param_type data )
            {
                node_type *node = query();
                try
                {
                    return new (node) node_type(data);
                }
                catch(...)
                {
                    store(node);
                    throw;
                }
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

            template <typename FUNCTION>
            inline void inorder( FUNCTION &F, const node_type *curr, const size_t level) const
            {
                if(curr)
                {
                    const size_t next_level = level+1;
                    inorder<FUNCTION>(F,curr->left,next_level);
                    F(curr->data,level);
                    inorder<FUNCTION>(F,curr->right,next_level);
                }
            }

            template <typename FUNCTION>
            inline void preorder( FUNCTION &F, const node_type *curr, const size_t level) const
            {
                if(curr)
                {
                    const size_t next_level = level+1;
                    F(curr->data,level);
                    preorder<FUNCTION>(F,curr->left,next_level);
                    preorder<FUNCTION>(F,curr->right,next_level);
                }
            }

            template <typename FUNCTION>
            inline void postorder( FUNCTION &F, const node_type *curr, const size_t level) const
            {
                if(curr)
                {
                    const size_t next_level = level+1;
                    postorder<FUNCTION>(F,curr->left,next_level);
                    postorder<FUNCTION>(F,curr->right,next_level);
                    F(curr->data,level);
                }
            }

        };

    };
}

#endif
