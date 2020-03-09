
//! \file

#ifndef Y_SUFFIX_TREE_INCLUDED
#define Y_SUFFIX_TREE_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/code/utils.hpp"
#include "y/ptr/auto.hpp"
#include "y/core/inode.hpp"

namespace upsylon {
    
    namespace core {
        
        //! base class for suffix tree
        class suffix_tree
        {
        public:
            class node_type : public object,
            public inode<node_type>,
            public ios::vizible
            {
            public:
                typedef core::list_of_cpp<node_type> children; //!< sub-trees
                node_type       *parent; //!< parent node
                void            *impl;   //!< address of data node
                const uint8_t    code;   //!< local code
                size_t           freq;   //!< local frequency for optimization
                children         chld;   //!< children
                
                
                explicit node_type(node_type *, const uint8_t) throw(); //!< setup
                virtual ~node_type() throw(); //!< cleanup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(node_type);
                virtual void vizCore(ios::ostream &) const;
            };
            
            typedef list<uint8_t>  path;     //!< used to rebuild keys
            
            virtual ~suffix_tree() throw();  //!< cleanup
            
            
            
        protected:
            explicit suffix_tree(); //!< setup
            static void throw_on_insert_failure(const path &); //!< common exception
            static void build_path_from(const node_type *curr,path &key);
            
            node_type           *root;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(suffix_tree);
        };
    }
    
    //! generic suffix tree
    template <typename T>
    class suffix_tree : public core::suffix_tree
    {
    public:
        Y_DECL_ARGS(T,type);                        //!< aliases
        
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
        
        
        //! setup
        inline explicit suffix_tree() : core::suffix_tree(), dlist(), dpool()
        {
        }
        
        //! cleaning up
        inline virtual ~suffix_tree() throw()
        {
            release_data();
        }
        
        //! hard copy
        inline suffix_tree( const suffix_tree &other ) :
        core::suffix_tree(), dlist(), dpool()
        {
            path key;
            for(const data_node *dnode=other.dlist.head;dnode;dnode=dnode->next)
            {
                other.build_path(key,dnode);
                if(!insert_with(key.begin(), key.size(), (**dnode).data))
                {
                    throw_on_insert_failure(key);
                }
            }
            assert( dlist.size == other.dlist.size );
            
        }
        
        //! accessing root
        inline const node_type & get_root() const throw() { return *root; }
        
        //! number of entries
        inline size_t entries() const throw() { return dlist.size; }
        
        
        //! type insertion with key iterator
        template <typename ITERATOR> inline
        bool insert_with(ITERATOR     key_data,
                         const size_t key_size,
                         param_type   args)
        {
            assert(root);assert(0==root->code);
            
            //------------------------------------------------------------------
            //
            // lool up for a current node
            //
            //------------------------------------------------------------------
            node_type *curr = root;
            for(size_t i=0;i<key_size;++i)
            {
                const uint8_t code = *(key_data++);
                bool          found = false;
                
                //--------------------------------------------------------------
                //
                // look for the code in child(ren)
                //
                //--------------------------------------------------------------
                node_type::children   &chld = curr->chld;
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
                    node_type           *parent   = curr->parent; curr->freq++;
                    node_type::children &siblings = parent->chld; assert(siblings.owns(curr));
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
                        curr=node;
                        found=true;
                        break;
                    }
                }
                if(!found)
                {
                    return NULL;
                }
            }
            assert(curr);
            if(curr->impl)
            {
                const data_node *dnode = static_cast<data_node *>(curr->impl);
                return &(dnode->data.data);
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
            assert(root);assert(0==root->code);
            node_type *curr = root;
            for(size_t i=0;i<key_size;++i)
            {
                const uint8_t  code  = *(key_data++);
                bool           found = false;
                for(node_type *node  = curr->chld.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        curr=node;
                        found=true;
                        break;
                    }
                }
                if(!found)
                {
                    return false;
                }
            }
            assert(curr);
            if(curr->impl)
            {
                data_node *d = static_cast<data_node *>(curr->impl);
                curr->impl   = NULL;
                self_destruct(d->data);
                dpool.store( dlist.unlink(d) );
                return true;
            }
            else
            {
                return false;
            }
        }
        
        
        
    private:
        Y_DISABLE_ASSIGN(suffix_tree);
        data_list            dlist;
        data_pool            dpool;
        
        inline void build_path( path &key, const data_node  *dnode) const
        {
            assert( NULL != dnode     );
            assert( dlist.owns(dnode) );
            build_path_from( &(dnode->data.hook), key );
        }
        
        
        inline void release_data() throw()
        {
            data_node::destruct(dlist);
            data_node::destruct(dpool);
        }
        
    };
    
}

#endif

