#ifndef Y_AVL_TREE_INCLUDED
#define Y_AVL_TREE_INCLUDED 1

#include "y/container/container.hpp"
#include "y/comparator.hpp"
#include "y/object.hpp"
#include "y/type/self-destruct.hpp"

namespace upsylon
{

    //! avl trees
    struct avl
    {
        static const int at_left  = -1; //!< indicator
        static const int balanced =  0; //!< indicator
        static const int at_right =  1; //!< indicator

        //! generic node
        template <typename T>
        class node
        {
        public:
            Y_DECL_ARGS(T,type); //!< alias
            node      *parent;   //!< parent node
            node      *left;     //!< left child node
            node      *right;    //!< right child node
            int        factor;   //!< current factor
            const_type data;     //!< user data hard copy

            //! setup
            inline  node(param_type user_data) : parent(0), left(0), right(0), factor(balanced), data(user_data) {}
            //! cleanup
            inline ~node() throw() {  }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node);
        };


        //! generic tree
        template <
        typename T,
        typename COMPARATOR = increasing_comparator<T>
        >
        class tree : public container
        {
        public:
            Y_DECL_ARGS(T,type);       //!< alias
            typedef node<T> node_type; //!< alias

            //! setup
            inline explicit tree() throw() : root(0), nodes(0), pool_node(0), pool_size(0), compare()
            {
            }

            //! cleanup
            inline virtual ~tree() throw()
            {
                release__();
            }

            inline virtual size_t size()     const throw() { return nodes; }            //!< dynamic interface: size
            inline virtual size_t capacity() const throw() { return nodes+pool_size; }  //!< dynanic interface: capacity
            inline virtual void   free()     throw()       { free__(root); }            //!< container interface: free data, keep memoru
            inline virtual void   release()  throw()       { release__();  }            //!< container interface, release resources
            inline virtual void   reserve(const size_t n)  { for(size_t i=n;i>0;--i) store( new_node() ); } //!< reserve nodes

            //! insert data
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


            template <typename FUNCTION> inline void inorder(FUNCTION &F)   const { inorder<FUNCTION>(F,root,0);   } //!< in   order traversal
            template <typename FUNCTION> inline void preorder(FUNCTION &F)  const { preorder<FUNCTION>(F,root,0);  } //!< pre  order traversal
            template <typename FUNCTION> inline void postorder(FUNCTION &F) const { postorder<FUNCTION>(F,root,0); } //!< post order traversal

        private:
            Y_DISABLE_COPY_AND_ASSIGN(tree);
            node_type *root;            //!< tree root
            size_t     nodes;           //!< nodes in tree
            node_type *pool_node;       //!< top of the pool
            size_t     pool_size;       //!< pool size
        public:                         //|
            mutable COMPARATOR compare; //!< data comparator
        private:

            inline void insert__( node_type *node, node_type * curr )
            {
                assert(node);
                assert(curr);
                if(compare(node->data,curr->data)<=0)
                {
                    // should insert at left
                    if(0==curr->left)
                    {
                        curr->left   = node;
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
                        curr->right  = node;
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
                    self_destruct(node->data);
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
                    self_destruct(node->data);
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
