
//! \file

#ifndef Y_SUFFIX_STORE_INCLUDED
#define Y_SUFFIX_STORE_INCLUDED 1

#include "y/associative/suffix/store-look-up.hpp"
#include "y/associative/suffix/stem.hpp"
#include "y/core/pool.hpp"
#include "y/type/args.hpp"
#include "y/object.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! fast suffix store to keep track of keys
    /**
     keys are sequences of integral type T
     */
    //
    //__________________________________________________________________________
    template <typename T>
    class suffix_store : public suffix_stem
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
            node_type  *next; //!< for list
            node_type  *prev; //!< for list
            node_list   chld; //!< for tree
            bool        used; //!< flag to check used/unused node
            const_type  code; //!< current code

            //! setup
            inline explicit node_type(const_type args) throw() :
            next(0), prev(0), chld(), used(false), code(args)
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
        inline explicit suffix_store() :
        root( create_node(0) ),
        cache()
        {
            aliasing::_(nodes) = 1;
        }

        //! cleanup
        inline virtual ~suffix_store() throw()
        {
            destruct();
        }

        //! copy
        inline suffix_store(const suffix_store &other) :
        root( create_node(0) ),
        cache()
        {
            aliasing::_(nodes) = 1;
            try {
                const node_type *curr = other.root;
                exit(1);
            }
            catch(...)
            {
                destruct();
                throw;
            }
        }

        //______________________________________________________________________
        //
        // stem interface
        //______________________________________________________________________
        //! free all sub-nodes
        inline virtual void free() throw()
        {
            root->free_into(cache);
            aliasing::_(nodes) = 1;
        }

        //! pre-allocate some nodes
        inline virtual void grow_cache(size_t n)
        {
            while( n-- > 0 ) cache.store( create_node(0) );
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
        //! generic insertion following an iterator
        template <typename ITERATOR> inline
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
                    curr = curr->chld.push_back( query_node(code) );
                    ++aliasing::_(nodes);
                }
            }
            assert(NULL!=curr);
            if(curr->used)
            {
                return false;
            }
            else
            {
                curr->used = true;
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
        Y_DISABLE_ASSIGN(suffix_store);
        node_type *root;

        inline void destruct() throw()
        {
            delete root;
            root = 0;
            aliasing::_(nodes) = created = 0;
        }

        inline node_type *create_node(const_type code)
        {
            node_type *node = new node_type(code);
            ++created;
            return node;
        }

        inline node_type *query_node(const_type code)
        {
            if(cache.size>0)
            {
                node_type *node         = cache.query();
                aliasing::_(node->code) = code;
                node->used              = false;
                return node;
            }
            else
            {
                return create_node(code);
            }
        }

    public:
        node_pool    cache; //!< cache of unused nodes
    };

}

#endif

