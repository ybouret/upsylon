
//! \file

#ifndef Y_SUFFIX_STORE_INCLUDED
#define Y_SUFFIX_STORE_INCLUDED 1

#include "y/associative/suffix/stem.hpp"
#include "y/associative/suffix/store-look-up.hpp"
#include "y/core/pool.hpp"
#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/object.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //! common stuff
    //__________________________________________________________________________
    struct suffix_store_
    {
        static const char className[]; //!< "suffix_store"
    };

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

            //! write as binary, return written bytes
            inline size_t save( ios::ostream &fp ) const
            {
                // write status
                fp.write( used ? 1 : 0 );

                // write code
                size_t written = 1 + fp.write_nbo(code);

                // write child(ren)
                written += fp.write_upack(chld.size);
                for(const node_type *node=chld.head;node;node=node->next)
                {
                    written += node->save(fp);
                }
                return written;
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

        //! copy by recursive duplication
        inline suffix_store(const suffix_store &other) :
        suffix_stem(),
        root( create_node(0) ),
        cache()
        {
            aliasing::_(nodes) = 1;
            try {
                duplicate(root,other.root);
                assert(is_same_than(other));
            }
            catch(...)
            {
                destruct();
                throw;
            }
        }

        //______________________________________________________________________
        //
        // serializable
        //______________________________________________________________________
        
        //! className
        inline virtual const char *className() const throw()
        {
            return suffix_store_::className;
        }

        //! serialize
        inline virtual size_t serialize(ios::ostream &fp) const
        {
            assert(root);
            return root->save(fp);
        }

        //______________________________________________________________________
        //
        // loading
        //______________________________________________________________________

        //! free and load serialized tree
        size_t load( ios::istream &fp )
        {
            free();
            try {
                assert(root);
                assert(0==root->chld.size);
                return load_node(root,fp);
            }
            catch(...)
            {
                free();
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
            aliasing::_(nodes)   = 1;
            aliasing::_(entries) = 0;
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
                ++aliasing::_(entries);
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

        //! comparison
        inline bool is_same_than( const suffix_store &other ) const
        {
            if(compare(root,other.root))
            {
                assert(nodes==other.nodes);
                return true;
            }
            else
            {
                return false;
            }
        }


    private:
        Y_DISABLE_ASSIGN(suffix_store);
        node_type *root;

        inline void destruct() throw()
        {
            delete root;
            root = 0;
            aliasing::_(nodes)   = 0;
            aliasing::_(entries) = 0;

        }

        inline node_type *create_node(const_type code)
        {
            node_type *node = new node_type(code);
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

        //! recursive duplication
        inline void duplicate(node_type       *target,
                              const node_type *source)
        {
            assert(0==cache_size());
            aliasing::_(target->code) = source->code;
            target->used              = source->used;

            node_list   &sub          = target->chld;
            for(const node_type *ch=source->chld.head;ch;ch=ch->next)
            {
                duplicate(sub.push_back( create_node(ch->code) ),ch);
                ++aliasing::_(nodes);
            }
        }

        //! in depth recursive comparision
        static inline bool compare(const node_type *lhs,
                                   const node_type *rhs)
        {
            if(lhs->code==rhs->code && lhs->used==rhs->used && lhs->chld.size == rhs->chld.size )
            {
                for(const node_type
                    *lsub = lhs->chld.head,
                    *rsub = rhs->chld.head;
                    lsub;
                    lsub=lsub->next,
                    rsub=rsub->next)
                {
                    if(!compare(lsub,rsub)) return false;
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        //! recursive loading
        inline size_t load_node( node_type *node, ios::istream &fp)
        {
            assert(node);
            assert(0==node->chld.size);
            size_t ans = 0;

            {
                char   flag  = -1;
                size_t shift = 0;
                if(!fp.query_nbo(flag,shift)) throw_missing(used);
                ans += shift;
                node->used = (flag!=0);
            }

            {
                size_t shift = 0;
                if(!fp.query_nbo(aliasing::_(node->code),shift)) throw_missing(code);
                ans += shift;
            }

            size_t nsub = 0;
            {
                size_t shift    = 0;
                if(!fp.query_upack(nsub,shift)) throw_missing(branches);
                ans += shift;
            }

            while(nsub-- > 0 )
            {
                ans += load_node(node->chld.push_back( query_node(0) ),fp);
                ++aliasing::_(nodes);
            }

            return ans;
        }

    public:
        node_pool    cache; //!< cache of unused nodes
    };

}

#endif

