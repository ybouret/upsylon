//! \file

#ifndef Y_SUFFIX_TREE_INCLUDED
#define Y_SUFFIX_TREE_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/code/utils.hpp"
#include "y/core/inode.hpp"
#include "y/iterate/linked.hpp"
#include "y/strfwd.hpp"

namespace upsylon {
    
    namespace core {
        
        //! base class for suffix tree
        class suffix_tree
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef list<uint8_t>                path;         //!< used to rebuild keys
            class                                node_type;    //!< forward declaration
            typedef core::list_of_cpp<node_type> node_list;    //!< for tree
            static  upsylon::string path2string(const path &); //!< helper
            
            //! internal node for tree
            class node_type : public object, public inode<node_type>, public ios::vizible
            {
            public:
                node_type       *parent;                       //!< parent node
                void            *impl;                         //!< address of data node
                const uint8_t    code;                         //!< local code
                size_t           freq;                         //!< local frequency for optimization
                node_list        chld;                         //!< children
                
                
                explicit node_type(node_type *, const uint8_t) throw(); //!< setup
                virtual ~node_type() throw();                           //!< cleanup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(node_type);
                virtual void vizCore(ios::ostream &) const;
            };

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            virtual    ~suffix_tree() throw();  //!< cleanup

            //! check for registered key
            template <typename ITERATOR> inline
            bool registered(ITERATOR     key_data,
                            const size_t key_size) const throw()
            {
                const node_type *node = find_node(key_data,key_size);
                return (NULL!=node) && (NULL!=node->impl);
            }

            //! check registered
            bool has( const void *key_addr, const size_t key_size ) const throw();

            //! check registered
            bool has( const char *text ) const throw();

            //! check registered
            bool has( const memory::ro_buffer &buffer ) const throw();


        protected:
            explicit    suffix_tree(); //!< setup
            node_type  *root;          //!< root node

            //! generic node finding following key
            template <typename ITERATOR> inline
            node_type *find_node(ITERATOR     key_data,
                                 const size_t key_size) const throw()
            {
                assert(root);assert(0==root->code);
                const node_type *curr = root;
                for(size_t i=0;i<key_size;++i)
                {
                    const uint8_t code = *(key_data++);
                    bool          found = false;
                    for(const node_type *node=curr->chld.head;node;node=node->next)
                    {
                        if(code==node->code)
                        {
                            curr  = node;
                            found = true;
                            break;
                        }
                    }
                    if(!found)
                    {
                        return NULL;
                    }
                }
                assert(curr);
                return (node_type *)curr;
            }

            //------------------------------------------------------------------
            //
            // static helpers
            //
            //------------------------------------------------------------------
            static void throw_on_insert_failure(const path &);         //!< common exception
            static void rebuild_path(path &key,const node_type *curr); //!< common rebuild of path

        private:
            Y_DISABLE_COPY_AND_ASSIGN(suffix_tree);
        };
    }
    
    //! alias to rebuild keys
    typedef core::suffix_tree::path suffix_path;
    
    //! generic suffix tree
    template <typename T>
    class suffix_tree : public core::suffix_tree
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type); //!< aliases
        
        //! a leave in a tree
        class leave
        {
        public:
            type             data; //!< the actual data
            const node_type &hook; //!< place in the tree

            //! setup
            inline  leave(const_type &args, const node_type &node) : data(args), hook(node) {}

            //! cleanup
            inline ~leave() throw() {}


        private:
            Y_DISABLE_COPY_AND_ASSIGN(leave);
        };
        
        typedef core::knode<leave>       data_node; //!< data node
        typedef core::list_of<data_node> data_list; //!< list of data
        typedef core::pool_of<data_node> data_pool; //!< pool of data

        //! accessing data from leave, for iterators
        struct leave_data {
            //! node->type
            static inline const_type &get(const data_node *node) throw() { assert(node); return node->data.data; }
        };

        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------
        
        //! setup
        inline explicit suffix_tree() : core::suffix_tree(), dlist(), dpool()
        {
        }

        //! setup with n data node
        inline explicit suffix_tree(const size_t n, const as_capacity_t &) :
        core::suffix_tree(), dlist(), dpool()
        {
            extra(n);
        }

        //! cleaning up
        inline virtual ~suffix_tree() throw()
        {
            release_data();
        }
        
        //! hard copy
        inline suffix_tree( const suffix_tree &other ) : core::suffix_tree(), dlist(), dpool()
        {
            path key;
            for(const data_node *dnode=other.dlist.head;dnode;dnode=dnode->next)
            {
                other.rebuild_path(key,&(dnode->data.hook));
                if(!insert_with(key.begin(), key.size(), (**dnode).data))
                {
                    throw_on_insert_failure(key);
                }
            }
            assert( dlist.size == other.dlist.size );
            
        }

        //----------------------------------------------------------------------
        //
        // methods
        //
        //----------------------------------------------------------------------

        //! append extra node
        inline void extra(const size_t n)
        {
            data_node::prefetch(dpool,n);
        }

        //! accessing root
        inline const node_type & get_root() const throw() { return *root; }
        
        //! number of entries
        inline size_t entries()    const throw() { return dlist.size; }

        //! number in cache
        inline size_t cache_size() const throw() { return  dpool.size; }


        //! dismiss all nodes
        inline void free_all() throw()
        {
            data_node::destruct_to(dpool,dlist);
            release_root();
        }

        //! release all data
        inline void release_all() throw()
        {
            release_data();
            release_root();
        }



        //! type insertion with key iterator
        template <typename ITERATOR> inline
        bool insert_with(ITERATOR     key_data,
                         const size_t key_size,
                         param_type   args)
        {
            assert(root);assert(0==root->code);
            
            //------------------------------------------------------------------
            //
            // look up for a current node
            //
            //------------------------------------------------------------------
            node_type *curr = root;
            for(size_t i=0;i<key_size;++i)
            {
                const uint8_t code  = *(key_data++);
                bool          found = false;
                
                //--------------------------------------------------------------
                //
                // look for the code in child(ren)
                //
                //--------------------------------------------------------------
                node_list     &chld = curr->chld;
                for(node_type *node = chld.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        curr   = node;
                        found  = true;
                        break;
                    }
                }
                
                //--------------------------------------------------------------
                //
                // check if a new child is to be set
                //
                //--------------------------------------------------------------
                if(!found)
                {
                    curr = curr->chld.push_back( new node_type(curr,code) );
                }
            }
            assert(NULL!=curr);
            
            if(NULL != curr->impl )
            {
                return false; // already someone
            }
            else
            {
                // create data
                curr->impl = dlist.push_back( data_node::create_alive_with(dpool,args,*curr) );
                assert(curr->impl!=NULL);
                
                // update frequencies and ordering
                while(curr->parent)
                {
                    node_type  *parent   = curr->parent; curr->freq++;
                    node_list  &siblings = parent->chld; assert(siblings.owns(curr));
                    while(curr->prev && curr->prev->freq<curr->freq)
                    {
                        siblings.towards_head(curr);
                    }
                    curr = parent;
                }
                return true;
            }
            
        }
        
        //! look up with key iterator
        template <typename ITERATOR> inline
        const_type *look_for(ITERATOR     key_data,
                             const size_t key_size) const throw()
        {
            const node_type *node = find_node(key_data,key_size);
            if(node&&node->impl)
            {
                const data_node *dnode = static_cast<const data_node *>(node->impl);
                const leave     &value = **dnode;
                return &(value.data);
            }
            else
            {
                return 0;
            }
        }
        
        //! remove with key iterator
        template <typename ITERATOR> inline
        bool remove_with(ITERATOR     key_data,
                         const size_t key_size) throw()
        {
            node_type *node = find_node(key_data,key_size);
            if(node&&node->impl)
            {
                data_node *d = dlist.unlink( static_cast<data_node *>(node->impl) );
                node->impl   = NULL;
                data_node::destruct(d,dpool);
                return true;
            }
            else
            {
                return false;
            }
        }



        //! sort data
        template <typename FUNC> inline
        void sort_with( FUNC &func )
        {
            merging<data_node>::sort( dlist, call_compare<FUNC>, (void*) &func );
        }

        //! insert with memory area as key
        inline bool insert_by(const void  *key_addr,
                              const size_t key_size,
                              param_type   args)
        {
            assert( !(NULL==key_addr&&key_size>0) );
            const uint8_t *key = static_cast<const uint8_t *>(key_addr);
            return insert_with(key,key_size,args);
        }

        //! insert with text
        inline bool insert_by(const char *text, param_type args)
        {
            return insert_by(text, text ? strlen(text) : 0, args);
        }

        //! insert with buffer
        inline bool insert_by(const memory::ro_buffer &buffer, param_type args)
        {
            return insert_by(buffer.ro(),buffer.length(),args);
        }

        //! remove with memory area as key
        inline bool remove_by(const void  *key_addr,
                              const size_t key_size) throw()
        {
            assert( !(NULL==key_addr&&key_size>0) );
            const uint8_t *key = static_cast<const uint8_t *>(key_addr);
            return remove_with(key,key_size);
        }


        //! remove with text
        inline bool remove_by(const char *text) throw()
        {
            return remove_by(text, text ? strlen(text) : 0);
        }

        //! remove with buffer
        inline bool remove_by(const memory::ro_buffer &buffer) throw()
        {
            return remove_by(buffer.ro(),buffer.length());
        }

        //! search with memory area as key
        inline const_type * search_by(const void  *key_addr,
                                      const size_t key_size) const throw()
        {
            assert( !(NULL==key_addr&&key_size>0) );
            const uint8_t *key = static_cast<const uint8_t *>(key_addr);
            return look_for(key,key_size);
        }

        //! search with text
        inline const_type *search_by(const char *text) const throw()
        {
            return search_by(text, text ? strlen(text) : 0);
        }

        //! search with buffer
        inline const_type *search_by(const memory::ro_buffer &buffer) const throw()
        {
            return search_by(buffer.ro(),buffer.length());
        }

        //! for_each func(suffix_key,data)
        template <typename FUNC>
        inline bool for_each( FUNC &func ) const
        {
            path key;
            for(const data_node *node=dlist.head;node;node=node->next)
            {
                const leave &l = node->data;
                rebuild_path(key,&(l.hook));
                if(!func(key,l.data)) return false;
            }
            return true;
        }
        
        const_type & back()
        {
            return dlist.tail->data.data;
        }
        
        void pop_back() throw()
        {
            assert(dlist.size);
            data_node *dnode = dlist.pop_back(); assert(dnode);
            node_type *tnode = (node_type*)&(dnode->data.hook); assert(tnode);
            data_node::destruct(dnode,dpool);
            tnode->impl = 0;
        }
        
    protected:
        data_list            dlist; //!< list of data nodes
        data_pool            dpool; //!< pool of data nodes
        
    private:
        Y_DISABLE_ASSIGN(suffix_tree);


        template <typename FUNC>
        static inline int call_compare( const data_node *lhs, const data_node *rhs, void *args )
        {
            FUNC &func = *(FUNC *)args;
            return func(lhs->data.data,rhs->data.data);
        }

        inline void release_root() throw()
        {
            root->chld.release();
            root->impl = NULL;
        }

        inline void release_data() throw()
        {
            data_node::destruct(dlist);
            data_node::destruct(dpool);
        }

    public:
        typedef iterate::linked<type,data_node,iterate::forward,leave_data>             iterator;        //!< forward iterator
        typedef iterate::linked<const_type,const data_node,iterate::forward,leave_data> const_iterator;  //!< forward iterator

        inline iterator       begin() throw() { return iterator(dlist.head); } //!< begin
        inline iterator       end()   throw() { return iterator(NULL);       } //!< end

        inline const_iterator begin() const throw() { return const_iterator(dlist.head); } //!< begin, const
        inline const_iterator end()   const throw() { return const_iterator(NULL);       } //!< end, const

        
    };
    
}

#endif

