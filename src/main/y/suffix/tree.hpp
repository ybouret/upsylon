
//! \file

#ifndef Y_SUFFIX_TREE_INCLUDED
#define Y_SUFFIX_TREE_INCLUDED 1

#include "y/suffix/node.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    
    //__________________________________________________________________________
    //
    //
    //! low level tree operations on anonymous user's data
    //
    //__________________________________________________________________________
    template <typename CODE>
    class suffix_tree
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef suffix_node<CODE>          tree_node; //!< alias
        typedef typename tree_node::pool_t tree_pool; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        inline explicit suffix_tree() :
        root( new tree_node(0,0,0) ),
        load(1),
        mark(0),
        pool()
        {
        }
        
        inline virtual ~suffix_tree() throw()
        {
            assert(root);
            delete (tree_node *)root;
            *(tree_node **)&root = 0;
            aliasing::_(load)    = 0;
        }
        
        //______________________________________________________________________
        //
        // cache operations
        //______________________________________________________________________
        
        //! sort cache
        inline void cache_sort() throw() { merging<tree_node>::sort_by_increasing_address(pool);   }
        
        //!  load empty nodes
        inline void cache_load(size_t n) { while(n-- > 0 ) pool.push_back( new tree_node(0,0,0) ); }
        
        //______________________________________________________________________
        //
        //! grow tree by range
        //______________________________________________________________________
        template <typename ITERATOR> inline
        bool insert_by(ITERATOR curr,
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
                node = node->leaves.push_back( tree_node::make(pool,node,code,0) );
                ++aliasing::_(load);
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
        
        //______________________________________________________________________
        //
        //! grow tree by sequence
        //______________________________________________________________________
        template <typename SEQUENCE> inline
        bool insert_by( SEQUENCE &seq, void *addr)
        {
            return insert_by( seq.begin(), seq.size(), addr);
        }
        
        //______________________________________________________________________
        //
        //! grow tree by accessible
        //______________________________________________________________________
        template <typename U> inline
        bool insert_at(const accessible<U> &path, void *addr)
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
                node = node->leaves.push_back( tree_node::make(pool,node,code,0) );
                ++aliasing::_(load);
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
        
        
        //______________________________________________________________________
        //
        //! search a node by range, result may be used or not!
        //______________________________________________________________________
        template <typename ITERATOR> inline
        const tree_node *find_by(ITERATOR     curr,
                                 size_t       size) const throw()
        {
            const tree_node *node = root;
            while(size-- > 0)
            {
                const CODE code = *(curr++);
                for(tree_node *chld=node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        node = chld;
                        goto FOUND;
                    }
                }
                return NULL;
            FOUND:;
                assert(node);
            }
            
            assert(node);
            return node;
        }
        
        //______________________________________________________________________
        //
        //! search a node by sequence, result may be used or not
        //______________________________________________________________________
        template <typename SEQUENCE> inline
        const tree_node *find_by(SEQUENCE &seq) const throw()
        {
            return find_by(seq.begin(),seq.size());
        }
        
        //______________________________________________________________________
        //
        //! search a node by path, result may be used or not
        //______________________________________________________________________
        template <typename U> inline
        const tree_node *find_at(const accessible<U> &path) const throw()
        {
            
            const tree_node *node = root;
            size_t           size = path.size();
            size_t           indx = 0;
            while(size-- > 0 )
            {
                const CODE code = path[++indx];
                for(tree_node *chld=node->leaves.head;chld;chld=chld->next)
                {
                    if(code==chld->code)
                    {
                        node = chld;
                        goto FOUND;
                    }
                }
                return NULL;
            FOUND:;
                assert(node);
            }
            assert(node);
            return node;
        }
        
        //______________________________________________________________________
        //
        //! pluck a used node from the tree
        //______________________________________________________________________
        inline void pluck(tree_node *node)  throw()
        {
            assert(node);
            assert(node->used!=0);
            
            node->used = 0;
            for(;;)
            {
                assert(node->frequency>0);
                const size_t  frequency = --(node->frequency);
                tree_node    *parent    = node->parent;
                if(parent)
                {
                    assert(parent->leaves.owns(node));
                    if(frequency<=0)
                    {
                        parent->leaves.unlink(node)->return_to(pool);
                        --aliasing::_(load);
                    }
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
        
        //______________________________________________________________________
        //
        //! try to find and pluck using a range
        //______________________________________________________________________
        template <typename ITERATOR>
        void *pull_by(ITERATOR     iter,
                      const size_t size) throw()
        {
            return cut( find_by(iter,size) );
        }
        
        //______________________________________________________________________
        //
        //! try to find and pluck using a sequence
        //______________________________________________________________________
        template <typename SEQUENCE>
        void *pull_by(SEQUENCE &seq) throw()
        {
            return cut( find_by(seq) );
        }
        
        //______________________________________________________________________
        //
        //! try to find and pluck using a path
        //______________________________________________________________________
        template <typename U>
        void *pull_at(const accessible<U> &path) throw()
        {
            return cut( find_at(path) );
        }
        
        //! erase content, keep memory
        inline void erase() throw()
        {
            ((tree_node *)root)->leaves_to(pool);
            zroot();
        }
        
        //! erase content, release all possible memory
        inline void ditch() throw()
        {
            pool.release();
            while(root->leaves.size) delete aliasing::_(root->leaves).pop_back();
            zroot();
        }
        
        //! clone another tree, this must be empty
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
                    aliasing::_(root->leaves).push_back( duplicate((tree_node*)root,node) );
                }
                aliasing::_(root->depth)     = other.root->depth;
                aliasing::_(root->code )     = other.root->code;
                aliasing::_(root->frequency) = other.root->frequency;
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
        
        //! check same layout=topology
        static inline
        bool have_same_layout(const suffix_tree &lhs,
                              const suffix_tree &rhs) throw()
        {
            return tree_node::have_same_layout(lhs.root,rhs.root);
        }
        
        //! check layouts
        inline friend bool operator==(const suffix_tree &lhs, const suffix_tree &rhs) throw()
        {
            return suffix_tree::have_same_layout(lhs,rhs);
        }
        
        //! check layouts
        inline friend bool operator!=(const suffix_tree &lhs, const suffix_tree &rhs) throw()
        {
            return !suffix_tree::have_same_layout(lhs,rhs);
        }
        
        //! get number of entries
        inline size_t size() const throw() { return root->frequency; }
        
        const tree_node * const root; //!< tree root
        const size_t            load; //!< total nodes
        tree_node              *mark; //!< last grow mark
        tree_pool               pool; //!< cache
        
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_tree);
        
        inline void *cut(const tree_node *node) throw()
        {
            if(node&&node->used)
            {
                void *addr = node->addr;
                pluck( (tree_node *)node );
                return addr;
            }
            else
            {
                return NULL;
            }
        }
        
        inline void zroot() throw()
        {
            assert(0==root->leaves.size);
            aliasing::_(root->frequency) = 0;
            aliasing::_(root->depth)     = 0;
            aliasing::_(root->code)      = 0;
            aliasing::_(load)            = 1;
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
            tree_node *node = tree_node::make(pool,parent,source->code,source->addr);
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
