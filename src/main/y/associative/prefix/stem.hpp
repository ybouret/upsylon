//! \file
#ifndef Y_PREFIX_STEM_INCLUDED
#define Y_PREFIX_STEM_INCLUDED 1

#include "y/associative/prefix/node.hpp"

namespace upsylon {
    
    //__________________________________________________________________________
    //
    //
    //! a stem is a basic dynamic tree
    //
    //__________________________________________________________________________
    template <typename CODE, typename T>
    class prefix_stem
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef          prefix_node<CODE,T>         node_type; //!< alias
        typedef typename prefix_node<CODE,T>::pool_t pool_type; //!< alias
        
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup with root node
        inline explicit prefix_stem() : root( new node_type(0,0) ), pool() {}
        
        //! cleanup
        inline virtual ~prefix_stem() throw() { delete root; root=0; }
        
        //______________________________________________________________________
        //
        //  methods
        //______________________________________________________________________
        
        //! get vizible root
        const ios::vizible & get_root() const throw() { assert(root); return *root; }
        
        //! tell number of inserted items
        inline size_t tell() const throw() { assert(root); return root->frequency; }
        
        
        
        //______________________________________________________________________
        //! grow current stem
        /**
         \param curr initial iterator, *curr must return a compatible type to CODE
         \param size size of path
         \param addr address to insert. TODO: NULL => used ?
         \param mark inserted node or busy node at path
         */
        //______________________________________________________________________
        template <typename ITERATOR>
        node_type *grow(ITERATOR    curr,
                        size_t      size,
                        T          *addr,
                        node_type **mark)
        {
            assert(0==root->code);
            assert(0==root->depth);
            assert(0!=mark);
            //------------------------------------------------------------------
            // start from root
            //------------------------------------------------------------------
            node_type *node = root;
            while(size-- > 0)
            {
                //----------------------------------------------------------
                // look for code
                //----------------------------------------------------------
                const CODE     code = *(curr++);
                for(node_type *chld = node->leaves.head;chld;chld=chld->next)
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
                node = node->leaves.push_back( new_node(node,code) );
                assert(false==node->used);
            FOUND:;
            }
            assert(node!=NULL);
            *mark = node;
            if(0!=node->used)
            {
                //--------------------------------------------------------------
                // busy!
                //--------------------------------------------------------------
                return NULL;
            }
            else
            {
                //--------------------------------------------------------------
                // assign address and optimize
                //--------------------------------------------------------------
                node->addr = addr;
                update_path_to(node);
                return node;
            }
        }
        
        
        //______________________________________________________________________
        //
        //! grow current stem using a sequence
        //______________________________________________________________________
        template <typename SEQUENCE> inline
        node_type *grow(SEQUENCE   &seq,
                        T          *addr,
                        node_type **mark)
        {
            return grow( seq.begin(), seq.size(), addr, mark);
        }
        
        //______________________________________________________________________
        //! grow current stem with a used node
        /**
         grow(curr,size,in_use_addr(),mark)
         */
        //______________________________________________________________________
        template <typename ITERATOR>
        node_type *tick(ITERATOR    curr,
                        size_t      size,
                        node_type **mark)
        {
            static T * _ = prefix_data<T>::in_use_addr();
            return grow(curr,size,_,mark);
        }
        
        //______________________________________________________________________
        //
        //! tick using a sequence
        //______________________________________________________________________
        template <typename SEQUENCE> inline
        node_type *tick(SEQUENCE   &seq,
                        node_type **mark)
        {
            return tick(seq.begin(),seq.size(),mark);
        }
        
        //______________________________________________________________________
        //
        //! search a node by path
        /**
         then node may be used or not!!
         */
        //______________________________________________________________________
        template <typename ITERATOR> inline
        const node_type *find(ITERATOR     curr,
                              size_t       size) const throw()
        {
            const node_type *node = root;
            while(size-- > 0)
            {
                const CODE code = *(curr++);
                for(node_type *chld=node->leaves.head;chld;chld=chld->next)
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
        //! search a node by sequence
        //______________________________________________________________________
        template <typename SEQUENCE> inline
        const node_type *find(SEQUENCE &seq) const throw()
        {
            return find(seq.begin(),seq.size());
        }
        
        //______________________________________________________________________
        //
        //! search a used node by path
        //______________________________________________________________________
        template <typename ITERATOR> inline
        bool has(ITERATOR     curr,
                 const size_t size) const throw()
        {
            const node_type *node = find(curr,size);
            return node && node->used!=0;
        }
        
        //______________________________________________________________________
        //
        //! search a used node by sequence
        //______________________________________________________________________
        template <typename SEQUENCE> inline
        bool has(SEQUENCE &seq) const throw()
        {
            return has(seq.begin(),seq.size());
        }
        
        
        //! pull a used node from the stem
        inline void pull(node_type *node)  throw()
        {
            assert(node);
            assert(node->used!=0);
            
            node->used = 0;
            for(;;)
            {
                assert(node->frequency>0);
                const size_t  frequency = --(node->frequency);
                node_type    *parent    = node->parent;
                if(parent)
                {
                    assert(parent->leaves.owns(node));
                    if(frequency<=0)
                    {
                        parent->leaves.unlink(node)->return_to(pool);
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
        
        //! return to single empty root, keep memory
        inline void reset() throw()
        {
            root->leaves_to(pool);
            zroot();
        }
        
        inline void ditch() throw()
        {
            while(root->leaves.size) delete root->leaves.pop_back();
            zroot();
        }
        
        //______________________________________________________________________
        //
        // cache control
        //______________________________________________________________________
        
        //! number of nodes in cache
        inline size_t cache_nodes() const throw() { return pool.size; }
        
        //!  grow cache with extra nodes
        inline void   cache_extra(size_t n)
        {
            while(n-- > 0) pool.push_back( new node_type(0,0) );
        }
        
        //! limit cache size
        inline void   cache_limit(const size_t n) throw()
        {
            if(n<=0)
            {
                pool.release();
            }
            else
            {
                merging<node_type>::sort_by_increasing_address(pool);
                while(pool.size>n) delete pool.pop_back();
            }
        }
        
        //! remove cache
        inline void cache_prune() throw() { cache_limit(0); }
        
        //______________________________________________________________________
        //
        //! duplicate
        //______________________________________________________________________
        inline void duplicate(const prefix_stem &other)
        {
            node_type *temp = duplicate(NULL,other.root);
            root->return_to(pool);
            root = temp;
            assert(node_type::have_same_layout(root,other.root));
        }
        
        //______________________________________________________________________
        //
        //! check the graph is the same
        //______________________________________________________________________
        inline bool similar_to(const prefix_stem &other) const throw()
        {
            return node_type::have_same_layout(root,other.root);
        }
        
        //______________________________________________________________________
        //
        //! no-throw exchange
        //______________________________________________________________________
        inline void xch(prefix_stem &other) throw()
        {
            cswap(root,other.root);
            pool.swap_with(other.pool);
        }
        
        //! func(node,args)
        template <typename FUNC, typename U>
        inline bool for_each(FUNC &func, U &args) const
        {
            return for_each(func,args,root);
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_stem);
        node_type *root;   //!< root node
        pool_type  pool;   //!< cached node
        
        void zroot() throw()
        {
            assert(0==root->leaves.size);
            root->frequency = 0;
            root->depth     = 0;
            root->code      = 0;
        }
        
        template <typename FUNC, typename U>
        bool for_each(FUNC &func, U &args, const node_type *node) const
        {
            if(node->used)
            {
                if(!func(node,args)) return false;
            }
            
            for(node=node->leaves.head;node;node=node->next)
            {
                if(!for_each(func,args,node) ) return false;
            }
            
            return true;
        }
        
        //! create a new node or format a stored one
        node_type *new_node(node_type *parent, const CODE code)
        {
            if(pool.size)
            {
                node_type *node = pool.pop_front();
                node->parent    = parent;
                node->code      = code;
                node->frequency = 0;
                node->depth     = parent->depth+1;
                node->used      = false;
                return node;
            }
            else
            {
                return new node_type(parent,code);
            }
        }
        
        void update_path_to(node_type *node) throw()
        {
            assert(node);
            for(;;)
            {
                ++(node->frequency);
                node_type *parent = node->parent;
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
        node_type *duplicate(node_type *parent, const node_type *source)
        {
            assert(source);
            node_type *node = new_node(parent,source->code);
            node->duplicate_status(source);
            try {
                for(const node_type *child=source->leaves.head;child;child=child->next)
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

