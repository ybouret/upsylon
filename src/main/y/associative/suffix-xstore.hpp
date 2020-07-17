
//! \file

#ifndef Y_SUFFIX_XSTORE_INCLUDED
#define Y_SUFFIX_XSTORE_INCLUDED 1

#include "y/associative/suffix-store-look-up.hpp"
#include "y/core/pool.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    //! fast suffix store to keep track of keys
    template <typename T>
    class suffix_xstore
    {
    public:
        Y_DECL_ARGS(T,type);                            //!< aliases
        class node_type;                                //!< forward declaration
        typedef core::list_of_cpp<node_type> node_list; //!< list of tree nodes
        typedef core::pool_of_cpp<node_type> node_pool; //!< pool of tree nodes

        //! tree node
        class node_type : public object
        {
        public:
            node_type  *next;   //!< for list
            node_type  *prev;   //!< for list
            node_type  *root;   //!< for tree
            size_t      freq;   //!< for tree
            node_list   chld;   //!< for tree
            bool        used;   //!< flag to check used/unused node
            const_type  code;   //!< current code

            //! setup
            inline explicit node_type(node_type *from,const_type args) throw() :
            next(0), prev(0), root(from), freq(0), chld(), used(false), code(args)
            {
            }
            
            //! cleanup
            inline virtual ~node_type() throw() { }

            //! recursive garbage collection
            void free_into(node_pool &cache) throw()
            {
                while(chld.size)
                {
                    cache.store(chld.pop_back())->free_into(cache);
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
        };

        //! setup
        inline explicit suffix_xstore() :
        root( new node_type(0,0) ),
        cache(),
        nodes(1)
        {
        }

        //! free all sub-nodes
        void free() throw()
        {
            root->free_into(cache);
            root->used = false;
            root->freq = 0;
            aliasing::_(nodes) = 1;
        }

        //! cleanup
        inline virtual ~suffix_xstore() throw()
        {
            delete root;
            root = 0;
            aliasing::_(nodes) = 0;
        }

        //! generic insertion following and iterator
        template <typename ITERATOR>
        bool insert(ITERATOR     path_iter,
                    size_t       path_size)
        {
            assert(root!=NULL);
            node_type *curr = root;

            //------------------------------------------------------------------
            //
            // walk down
            //
            //------------------------------------------------------------------
            while(path_size-- > 0 )
            {
                const_type code  = static_cast<const_type>(*(path_iter++));
                bool       found = false;
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
                    curr = curr->chld.push_back( query_node(curr,code) );
                    ++aliasing::_(nodes);
                }
            }
            assert(NULL!=curr);
            if(curr->used)
            {
                assert(curr->freq>0);
                return false;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // optimise
                //
                //--------------------------------------------------------------
                curr->used = true;
                while(curr->root)
                {
                    node_list &owner = curr->root->chld; assert(owner.owns(curr));
                    curr->freq++;
                    while(curr->prev && curr->prev->freq<curr->freq)
                    {
                        owner.towards_head(curr);
                    }
                    curr = curr->root;
                }
                return true;
            }
        }
        
        //! check if present
        template <typename ITERATOR> inline
        bool has(ITERATOR     path_iter,
                 size_t       path_size) const throw()
        {
            return
            core::suffix_store::look_up<mutable_type,ITERATOR,node_type>(root,path_iter,path_size);
        }

        
        //! pre-allocate some nodes
        inline void reserve(size_t n)
        {
            while( n-- > 0 ) cache.store( new node_type(0,0) );
        }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_xstore);
        node_type *root;

        inline node_type *query_node(node_type *from, const_type code )
        {
            if(cache.size>0)
            {
                node_type *node = cache.query();
                aliasing::_(node->code) = code;
                node->root              = from;
                node->freq              = 0;
                node->used              = false;
                return node;
            }
            else
            {
                return new node_type(from,code);
            }
        }

    public:
        node_pool    cache; //!< cache of unused nodes
        const size_t nodes; //!< number of nodes into tree
    };

}

#endif

