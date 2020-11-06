
//! \file

#ifndef Y_SUFFIX_TREE_INCLUDED
#define Y_SUFFIX_TREE_INCLUDED 1

#include "y/suffix/node.hpp"

namespace upsylon
{

    template <typename CODE>
    class suffix_tree
    {
    public:
        typedef suffix_node<CODE>          tree_node;
        typedef typename tree_node::pool_t tree_pool;

        inline explicit suffix_tree() :
        root( new tree_node(0,0,0) ),
        mark(0),
        pool()
        {
        }

        inline virtual ~suffix_tree() throw()
        {
            assert(root);
            delete (tree_node *)root;
            *(tree_node **)&root = 0;
        }


        inline void sort_pool() throw() { merging<tree_node>::sort_by_increasing_address(pool); }

        template <typename ITERATOR>
        bool grow(ITERATOR curr,
                  size_t   size,
                  void    *addr)
        {
            assert(0==root->code);
            assert(0==root->depth);
            assert(addr!=NULL);

            //------------------------------------------------------------------
            // start from root
            //------------------------------------------------------------------
            tree_node *node = (tree_node*)root;
            mark            = 0;
            while(size-- > 0)
            {
                //----------------------------------------------------------
                // look for code
                //----------------------------------------------------------
                const CODE     code = *(curr++);
                for(tree_node *chld = node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        //------------------------------------------------------
                        // take this path
                        //------------------------------------------------------
                        node = chld;
                        goto FOUND;
                    }
                }
                //--------------------------------------------------------------
                // need a new leaf
                //--------------------------------------------------------------
                node = node->new_free_child(pool,code);
                assert(false==node->used);
            FOUND:;
            }
            assert(node!=NULL);
            mark = node;
            if(0!=node->used)
            {
                //--------------------------------------------------------------
                // busy!
                //--------------------------------------------------------------
                return false;
            }
            else
            {
                //--------------------------------------------------------------
                // assign address and optimize
                //--------------------------------------------------------------
                node->addr = addr;
                update_path_to(node);
                return true;
            }
        }

        template <typename U> inline
        bool grow(const accessible<U> &path, void *addr)
        {
            assert(0==root->code);
            assert(0==root->depth);
            assert(addr!=NULL);

            //------------------------------------------------------------------
            // start from root
            //------------------------------------------------------------------
            size_t     size = path.size();
            size_t     indx = 0;
            tree_node *node = (tree_node*)root;
            mark            = 0;
            while(size-- > 0)
            {
                //----------------------------------------------------------
                // look for code
                //----------------------------------------------------------
                const CODE     code = path[++indx];
                for(tree_node *chld = node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        //------------------------------------------------------
                        // take this path
                        //------------------------------------------------------
                        node = chld;
                        goto FOUND;
                    }
                }
                //--------------------------------------------------------------
                // need a new leaf
                //--------------------------------------------------------------
                node = node->new_free_child(pool,code);
                assert(false==node->used);
            FOUND:;
            }
            assert(node!=NULL);
            mark = node;
            if(0!=node->used)
            {
                //--------------------------------------------------------------
                // busy!
                //--------------------------------------------------------------
                return false;
            }
            else
            {
                //--------------------------------------------------------------
                // assign address and optimize
                //--------------------------------------------------------------
                node->addr = addr;
                update_path_to(node);
                return true;
            }
        }


        inline void erase() throw()
        {
            root->leaves_to(pool);
            zroot();
        }

        inline void ditch() throw()
        {
            pool.release();
            while(root->leaves.size) delete root->leaves.pop_back();
            zroot();
        }

        inline void clone(const suffix_tree &other)
        {
            assert(0==root->frequency);
            assert(0==root->leaves.size);
            assert(0==root->depth);
            assert(0==root->code);
            try
            {
                for(const tree_node *node=other.root->leaves.head;node;node=node->next)
                {
                    root->leaves.push_back( duplicate(root,node) );
                }
                root->depth     = other.root->depth;
                root->code      = other.root->code;
                root->frequency = other.root->frequency;
            }
            catch(...)
            {
                erase();
                throw;
            }
        }

        //! no-throw swap
        void swap_with(suffix_tree &other) throw()
        {
            _cswap(root,other.root);
            _cswap(mark,other.mark);
            pool.swap_with(other.pool);
        }

        static inline
        bool have_same_layout(const suffix_tree &lhs,
                              const suffix_tree &rhs) throw()
        {
            return tree_node::have_same_layout(lhs.root,rhs.root);
        }

        inline friend bool operator==(const suffix_tree &lhs, const suffix_tree &rhs) throw()
        {
            return suffix_tree::have_same_layout(lhs,rhs);
        }

        inline friend bool operator!=(const suffix_tree &lhs, const suffix_tree &rhs) throw()
        {
            return !suffix_tree::have_same_layout(lhs,rhs);
        }


        const tree_node * const root;
        tree_node              *mark;
        tree_pool               pool;

        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_tree);

        void zroot() throw()
        {
            assert(0==root->leaves.size);
            root->frequency = 0;
            root->depth     = 0;
            root->code      = 0;
        }

        void update_path_to(tree_node *node) throw()
        {
            assert(node);
            for(;;)
            {
                ++(node->frequency);
                tree_node *parent = node->parent;
                if(parent)
                {
                    assert(parent->leaves.owns(node));
                    parent->optimize();
                    node=parent;
                }
                else
                {
                    assert(node==root);
                    return;
                }
            }
        }

        //! recursive layout duplication
        tree_node *duplicate(tree_node *parent, const tree_node *source)
        {
            assert(source);
            assert(parent);
            tree_node *node = parent->new_free_child(pool,source->code);
            assert(node->depth=source->depth);
            assert(node->code==source->code);
            node->frequency  = source->frequency;
            try {
                for(const tree_node *child=source->leaves.head;child;child=child->next)
                {
                    node->leaves.push_back( duplicate(node,child) );
                }
            } catch(...) {
                delete node;
                throw;
            }
            return node;
        }

    };

}

#endif
