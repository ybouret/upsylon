
//! \file

#ifndef Y_SUFFIX_XSTORE_INCLUDED
#define Y_SUFFIX_XSTORE_INCLUDED 1

#include "y/associative/suffix/store-look-up.hpp"
#include "y/associative/suffix/stem.hpp"
#include "y/core/pool.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {


    //__________________________________________________________________________
    //
    //! common stuff
    //__________________________________________________________________________
    struct suffix_xstore_
    {
        static const char className[]; //!< "suffix_xstore"
    };

    //__________________________________________________________________________
    //
    //
    //! optimized suffix store to keep track of keys
    /**
     keys are sequences of T
     */
    //
    //__________________________________________________________________________
    template <typename T>
    class suffix_xstore : public suffix_stem
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                            //!< aliases
        class node_type;                                //!< forward declaration
        typedef core::list_of_cpp<node_type> node_list; //!< list of tree nodes
        typedef core::pool_of_cpp<node_type> node_pool; //!< pool of tree nodes

        //______________________________________________________________________
        //
        //! tree node
        //______________________________________________________________________
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

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup
        inline explicit suffix_xstore() :
        root( create_node(0,0) ),
        cache()
        {
            aliasing::_(nodes) = 1;
        }

        //! cleanup
        inline virtual ~suffix_xstore() throw()
        {
            delete root;
            root = 0;
            aliasing::_(nodes) = 0;
        }

        //______________________________________________________________________
        //
        // serializable
        //______________________________________________________________________
        //! className
        inline virtual const char *className() const throw()
        {
            return suffix_xstore_::className;
        }

        //! save
        inline virtual size_t serialize(ios::ostream &) const
        {

            return 0;
        }


        //______________________________________________________________________
        //
        // stem inferface
        //______________________________________________________________________
        //! free all sub-nodes
        inline virtual void free() throw()
        {
            root->free_into(cache);
            root->used = false;
            root->freq = 0;
            aliasing::_(nodes) = 1;
        }

        //! pre-allocate some nodes
        inline virtual void grow_cache(size_t n)
        {
            while( n-- > 0 ) cache.store( create_node(0,0) );
        }

        //! trim cache
        inline virtual void free_cache() throw()
        {
            cache.release();
        }

        //! cached nodes
        inline virtual size_t cache_size() const throw()
        {
            return cache.size;
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
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

        

        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_xstore);
        node_type *root;

        inline node_type *create_node(node_type *from, const_type code)
        {
            node_type *node = new node_type(from,code);
            ++created;
            return node;
        }

        inline node_type *query_node(node_type *from, const_type code)
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
                return create_node(from,code);
            }
        }

    public:
        node_pool    cache; //!< cache of unused nodes
    };

}

#endif

