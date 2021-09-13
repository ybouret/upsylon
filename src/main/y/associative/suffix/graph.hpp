//! \file

#ifndef Y_SUFFIX_GRAPH_INCLUDED
#define Y_SUFFIX_GRAPH_INCLUDED 1

#include "y/associative/suffix/knot.hpp"
#include "y/associative/suffix/tree.hpp"
#include "y/associative/suffix/collection.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/type/aliasing.hpp"
#include "y/memory/cppblock.hpp"
#include "y/memory/buffer-of.hpp"
#include "y/iterate/linked.hpp"
#include "y/core/nodes.hpp"

namespace upsylon
{


    //__________________________________________________________________________
    //
    //
    //! compound class to build suffix classes
    /**
     handle a list of data matching an internal suffix_tree.
     derived class:
     - suffix_storage for strings/buffer keys
     - suffix_batch for catalog interface
     - suffix_table for lexicon interface
     - ledger to store addresses
     */
    //
    //__________________________________________________________________________
    template <
    typename CODE,
    typename T,
    typename BASE_CLASS = suffix_collection>
    class suffix_graph : public BASE_CLASS
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                                   //!< aliases
        typedef suffix_tree<CODE>                  tree_type;  //!< alias
        typedef suffix_node<CODE>                  tree_node;  //!< alias
        typedef suffix_knot<T>                     data_node;  //!< alias
        typedef typename data_node::list_type      data_list;  //!< alias
        typedef typename data_node::pool_type      data_pool;  //!< alias
        typedef memory::cppblock<CODE>             code_block; //!< block for reusable local keys
        typedef memory::pooled                     key_memory; //!< to extract keys
        typedef memory::buffer_of<CODE,key_memory> key_path;   //!< exact key_path extraction
        typedef core::doubly<key_path>             key_node;   //!< to store keys
        typedef core::list_of_cpp<key_node>        raw_keys;   //!< to store keys

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup
        inline explicit       suffix_graph() : BASE_CLASS(), dlist(), htree(), dpool() {}

        //! setup with capacity
        inline explicit suffix_graph(const size_t n, const as_capacity_t &) : BASE_CLASS(), dlist(), htree(), dpool()
        {
            dpool.cache(n);
        }

        //! copy
        inline suffix_graph(const suffix_graph &other) : collection(), BASE_CLASS(), dlist(), htree(), dpool()
        {
            const size_t nmax = other.max_depth();
            code_block   code(nmax);

            for(const data_node *node=other.dlist.head;node;node=node->next)
            {
                const size_t len = static_cast<const tree_node *>(node->hook)->encode(code);
                this->insert_by(*code,len,**node);
            }
            assert(this->has_same_layout_than(other));
        }

        //! assign by copy/swap
        inline suffix_graph & operator=( const suffix_graph &other )
        {
            suffix_graph temp(other);
            swap_with(temp);
            return *this;
        }

        //! cleanup
        inline virtual ~suffix_graph() throw() {}
        
        //______________________________________________________________________
        //
        // container interface if necessary
        //______________________________________________________________________

        //! number of objects
        inline virtual size_t size()     const throw() { return dlist.size; }
        
        //! capacity
        inline virtual size_t capacity() const throw() { return dlist.size + dpool.size; }
        
        //! preload cache of data_node
        inline virtual void   reserve(const size_t n) { dpool.cache(n); }


        //! remove objects, keep memory
        inline virtual void   free() throw()
        {
            dpool.store(dlist);
            htree.erase();
        }
        
        //! release all possible memory
        inline virtual void release() throw()
        {
            dpool.release();
            dlist.release();
            htree.ditch();
        }


        //______________________________________________________________________
        //
        // lower level functions
        //______________________________________________________________________


        //! preload cache of tree_nod
        inline void           reserve_leaves(const size_t n) { htree.cache_load(n); }


        //! same layout indepently of data
        inline bool has_same_layout_than(const suffix_graph &rhs) const throw()
        {
            return tree_type::have_same_layout(htree,rhs.htree);
        }

        //! maximum depth
        inline size_t max_depth() const throw()
        {
            size_t ans = 0;
            for(const data_node *node=dlist.head;node;node=node->next)
            {
                assert(node->hook);
                const size_t depth = static_cast<const tree_node *>(node->hook)->depth;
                if(depth>ans) ans = depth;
            }
            return ans;
        }

        //! get last insertion trial
        const tree_node *mark() const throw() { return htree.mark; }

        //! no-throw swap
        inline void swap_with(suffix_graph &g) throw()
        {
            dpool.swap_with(g.dpool);
            dlist.swap_with(g.dlist);
            htree.swap_with(g.htree);
        }
        
        //______________________________________________________________________
        //
        // low-level insert
        //______________________________________________________________________

        //! calling htree methods
#define Y_SUFFIX_GRAPH_INSERT(CALL)                              \
data_node *node = dpool.query(args);                             \
try { return htree.CALL ? on_success(node) : on_failure(node); } \
catch(...) { dpool.store(node); throw; }

        //! insert data by range
        template <typename ITERATOR> inline
        bool insert_by(ITERATOR     curr,
                       const size_t size,
                       param_type   args)
        {
            Y_SUFFIX_GRAPH_INSERT(insert_by(curr,size,node))
        }

        //! insert data by sequence
        template <typename SEQUENCE> inline
        bool insert_by(SEQUENCE &seq, param_type args)
        {
            Y_SUFFIX_GRAPH_INSERT(insert_by(seq,node))
        }

        //! insert data by path
        template <typename U> inline
        bool insert_at(const accessible<U> &path, param_type args)
        {
            Y_SUFFIX_GRAPH_INSERT(insert_at(path,node))
        }
        
        //______________________________________________________________________
        //
        // low-level search
        //______________________________________________________________________

        //! search data by range
        template <typename ITERATOR> inline
        const_type *search_by(ITERATOR     curr,
                              const size_t size) const throw()
        {
            return data_from( htree.find_by(curr,size) );
        }

        //! search data by sequence
        template <typename SEQUENCE> inline
        const_type *search_by(SEQUENCE &seq) const throw()
        {
            return data_from( htree.find_by(seq) );
        }

        //! search data by path
        template <typename U> inline
        const_type *search_at(const accessible<U> &path) const throw()
        {
            return data_from( htree.find_at(path) );
        }
        
        //______________________________________________________________________
        //
        // low-level remove
        //______________________________________________________________________

        //! remove data by range
        template <typename ITERATOR> inline
        bool remove_by(ITERATOR     curr,
                       const size_t size) throw()
        {
            return try_remove( htree.pull_by(curr,size) );
        }

        //! remove data by sequence
        template <typename SEQUENCE> inline
        bool remove_by(SEQUENCE &seq) throw()
        {
            return try_remove( htree.pull_by(seq) );
        }

        //! remove data by path
        template <typename U> inline
        bool remove_at(const accessible<U> &path) throw()
        {
            return try_remove( htree.pull_at(path) );
        }

        //______________________________________________________________________
        //
        // sorting data
        //______________________________________________________________________

        //! wrapper to sort data with func
        template <typename FUNC>
        void sort_with( FUNC &func )
        {
            merge_list_of<data_node>::sort(dlist,compare_with<FUNC>,(void*)&func);
        }



        //______________________________________________________________________
        //
        // iterators
        //______________________________________________________________________

        typedef iterate::linked<type,data_node,iterate::forward>             iterator;        //!< forward iterator
        typedef iterate::linked<const_type,const data_node,iterate::forward> const_iterator;  //!< forward const iterator

        iterator begin() throw() { return iterator( dlist.head ); } //!< begin forward
        iterator end()   throw() { return iterator(0);            } //!< end forward

        const_iterator begin() const throw()   { return const_iterator( dlist.head ); } //!< begin forward const
        const_iterator end()   const throw()   { return const_iterator(0);            } //!< end forward const

        //! iterator -> tree_node
        static inline tree_node       *iter_node(iterator &it) throw() { return static_cast<tree_node*>(it.get().hook); }

        //! iterator -> tree_node
        static inline const tree_node *iter_node(const_iterator &it) throw() { return static_cast<const tree_node*>(it.get().hook); }

        //______________________________________________________________________
        //
        //! accessing root
        //______________________________________________________________________
        const tree_node &get_root() const throw() { return *htree.root; }


        //______________________________________________________________________
        //
        //! accessing tree
        //______________________________________________________________________
        const tree_type &get_tree() const throw() { return htree; }

        //______________________________________________________________________
        //
        //! printing
        //______________________________________________________________________
        inline std::ostream & display(std::ostream &os) const
        {
            const size_t dmax = max_depth();
            code_block   code(dmax);
            os << '{' << std::endl;
            for(const data_node *node=dlist.head;node;node=node->next)
            {
                const size_t len = static_cast<const tree_node *>(node->hook)->encode(code);
                os << ' ';
                for(size_t i=1;i<=len;++i)    os << code[i];
                for(size_t i=len;i<=dmax;++i) os << ' ';
                os << ':' << ' ';
                os << **node;
                os << std::endl;
            }
            os << '}';
            return os;
        }

        //! display
        inline friend std::ostream & operator<<( std::ostream &os, const suffix_graph &g)
        {
            return g.display(os);
        }

        //! stats
        inline void stats(std::ostream &os) const
        {
            os << "<suffix_graph>" << std::endl;
            os << "\tdata size = " << dlist.size      << std::endl;
            os << "\tdata pool = " << dpool.size      << std::endl;
            os << "\ttree load = " << htree.load      << std::endl;
            os << "\ttree pool = " << htree.pool.size << std::endl;
            os << "<suffix_graph/>" << std::endl;
        }

        //______________________________________________________________________
        //
        //! merging (logical OR on KEYS), return the number of added data
        //______________________________________________________________________
        template <typename OTHER_BASE_CLASS>
        inline size_t merge(const suffix_graph<CODE,T,OTHER_BASE_CLASS> &other)
        {
            size_t             num = 0;
            code_block         blk(other.max_depth());
            for(const data_node *node=other.head();node;node=node->next)
            {
                const size_t len = static_cast<const tree_node *>(node->hook)->encode(blk);
                if(insert_by(*blk,len,**node)) ++num;
            }
            return num;
        }

        //______________________________________________________________________
        //
        //! excluding (logical NOT on KEYS), return the number of excluded data
        //______________________________________________________________________
        template <typename OTHER_BASE_CLASS>
        inline size_t exclude(const suffix_graph<CODE,T,OTHER_BASE_CLASS> &other)
        {
            size_t             num = 0;
            code_block         blk(other.max_depth());
            const data_node   *node = other.head();
            while(node)
            {
                const data_node *next = node->next;
                const size_t len = static_cast<const tree_node *>(node->hook)->encode(blk);
                if(remove_by(*blk,len,**node)) ++num;
                node=next;
            }
            return num;
        }



        //______________________________________________________________________
        //
        //! collect all keys
        /**
         - KEY_TYPE must have a KEY_TYPE(size,as_capacity) constructor
         - KEY_TYPE must have a << operator
         - SEQ_TYPE must have a << operator
         */
        //______________________________________________________________________
        template < typename KEY_TYPE, typename SEQ_TYPE>
        inline void collect(SEQ_TYPE &keys) const
        {
            code_block blk(max_depth());
            for(const data_node *node=dlist.head;node;node=node->next)
            {
                const size_t len = static_cast<const tree_node *>(node->hook)->encode(blk);
                KEY_TYPE     key(len,as_capacity);
                for(size_t i=1;i<=len;++i) key << blk[i];
                keys << key;
            }
        }

        //______________________________________________________________________
        //
        //! collect all keys
        //______________________________________________________________________
        inline void collect_keys(raw_keys &keys) const
        {
            code_block     code(max_depth());
            for(const data_node *node=dlist.head;node;node=node->next)
            {
                size_t    len = static_cast<const tree_node *>(node->hook)->encode(code);
                key_path &key = *keys.push_back( key_node::template from<size_t>(len) );
                while(len>0)
                {
                    key[len] = code[len];
                    --len;
                }
            }
        }

        //______________________________________________________________________
        //
        //! remove if is_bad(key,data)
        //______________________________________________________________________
        template <typename IS_BAD>
        inline size_t remove_if(IS_BAD &is_bad)
        {
            size_t num = 0;

            code_block         code(max_depth());
            const data_node   *node = dlist.head;
            while(node)
            {
                const data_node *next = node->next;
                size_t           len = static_cast<const tree_node *>(node->hook)->encode(code);
                key_path         key(len);
                while(len>0) { key[len] = code[len]; --len; }
                const_type     &data = **node;
                if(is_bad(key,data))
                {
                    remove_at(key);
                    ++num;
                }
                node=next;
            }
            return num;
        }

        //______________________________________________________________________
        //
        //! direct access to head for internal faster scan
        //______________________________________________________________________
        inline const data_node *head() const throw() { return dlist.head; }
       
        //______________________________________________________________________
        //
        //! direct access to tail for internal faster scan
        //______________________________________________________________________
        inline const data_node *tail() const throw() { return dlist.tail; }

    private:
        data_list dlist;
        tree_type htree;
        data_pool dpool;

        template <typename FUNC> static inline
        int compare_with( const data_node *lhs, const data_node *rhs, void *args)
        {
            assert(lhs); assert(rhs); assert(args);
            FUNC &func = *(FUNC*)args;
            return func(lhs->data,rhs->data);
        }

        inline bool try_remove(void *addr) throw()
        {
            if(addr)
            {
                dpool.store( dlist.unlink( static_cast<data_node *>(addr) ) );
                assert(htree.size()==dlist.size);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        static inline type *data_from(const tree_node *node) throw()
        {
            if(node&&node->addr)
            {
                void *addr = (void *)(node->addr);
                return & static_cast<data_node *>(addr)->data;
            }
            else
            {
                return NULL;
            }
        }
        
        
        inline bool on_failure(data_node *node) throw()
        {
            assert(node);
            dpool.store(node);
            assert(dlist.size==htree.size());
            return false;
        }
        
        inline bool on_success(data_node *node) throw()
        {
            assert(node);
            assert(htree.mark);
            dlist.push_back(node)->hook = htree.mark;
            assert(dlist.size==htree.size());
            return true;
        }
        
    };
    
}

#endif
