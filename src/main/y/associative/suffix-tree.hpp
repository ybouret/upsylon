
//! \file

#ifndef Y_SUFFIX_TREE_INCLUDED
#define Y_SUFFIX_TREE_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/code/utils.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace core {

        //! base class for suffix tree
        class suffix_tree
        {
        public:
            typedef list<uint8_t>  path;     //!< used to rebuild keys
            virtual ~suffix_tree() throw();  //!< cleanup

        protected:
            explicit suffix_tree() throw(); //!< setup
            static void throw_on_insert_failure(const path &); //!< common exception

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
        class tree_node;                            //!< forward
        //! a leave in a tree
        class leave
        {
        public:
            type             data; //!< the actual data
            const tree_node &hook; //!< place in the tree
            //! setup
            inline  leave(const_type &args, const tree_node &node) : data(args), hook(node) {}
            //! cleanup
            inline ~leave() throw() {}
        private:
            Y_DISABLE_COPY_AND_ASSIGN(leave);
        };

        typedef core::knode<leave>       data_node; //!< data node
        typedef core::list_of<data_node> data_list; //!< list of data
        typedef core::pool_of<data_node> data_pool; //!< pool of data

        //! a tree node
        class tree_node : public object, public ios::vizible
        {
        public:
            typedef core::list_of_cpp<tree_node> children; //!< sub-trees
            const tree_node *parent; //!< parent node
            tree_node       *next;   //!< next for children
            tree_node       *prev;   //!< prev for children
            data_node       *addr;   //!< address of data node
            const uint8_t    code;   //!< local code
            size_t           freq;   //!< local frequency for optimization
            children         chld;   //!< children


            //! setup
            inline explicit tree_node(const tree_node *from, const uint8_t value) throw() :
            parent(from), next(0), prev(0), addr(0), code(value), freq(0), chld() {}

            //! cleanup
            inline virtual ~tree_node() throw()
            {

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(tree_node);
            inline virtual void vizCore( ios::ostream &fp ) const
            {
                const char *info = (addr!=0) ? "(*)" : "";
                if(parent)
                {
                    endl( fp( " [label=\"%s#%u%s\"]", printable_char[code], unsigned(freq),info) );
                }
                else
                {
                    endl(fp( " [label=\"root%s\"]",info));
                }
                for(const tree_node *node=chld.head;node;node=node->next)
                {
                    node->vizSave(fp);
                    endl(vizJoin(fp,node));
                }
            }

        };


        //! setup
        inline explicit suffix_tree() : root( new tree_node(0,0) ), dlist(), dpool()
        {
        }

        //! cleaning up
        inline virtual ~suffix_tree() throw()
        {
            release_data();
            delete root;
            root = 0;
        }

        //! hard copy
        inline suffix_tree( const suffix_tree &other ) :
        core::suffix_tree(), root( new tree_node(0,0) ), dlist(), dpool()
        {
            try {
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
            catch(...)
            {
                delete root;
                throw;
            }
        }

        //! accessing root
        inline const tree_node & get_root() const throw() { return *root; }

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
            tree_node *curr = root;
            for(size_t i=0;i<key_size;++i)
            {
                const uint8_t code = *(key_data++);
                bool          found = false;

                //--------------------------------------------------------------
                //
                // look for the code in child(ren)
                //
                //--------------------------------------------------------------
                typename tree_node::children   &chld = curr->chld;
                for(tree_node *node = chld.head;node;node=node->next)
                {
                    if(code==node->code)
                    {
                        curr   = node;
                        found  = true;
                        ++(curr->freq);
                        while(curr->prev&&curr->prev->freq<curr->freq)
                        {
                            chld.towards_head(curr);
                        }
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
                    (curr = curr->chld.push_back( new tree_node(curr,code) ))->freq++;
                }
            }
            assert(NULL!=curr);
            
            if(NULL != curr->addr )
            {
                return false; // already someone
            }
            else
            {
                curr->addr = dlist.push_back( data_node::create_alive_with(dpool,args,*curr) );
                assert(curr->addr!=NULL);
                return true;
            }

        }

        //! look up with key iterator
        template <typename ITERATOR> inline
        const_type *look_for(ITERATOR     key_data,
                             const size_t key_size) const throw()
        {
            assert(root);assert(0==root->code);
            const tree_node *curr = root;
            for(size_t i=0;i<key_size;++i)
            {
                const uint8_t code = *(key_data++);
                bool          found = false;
                for(const tree_node *node=curr->chld.head;node;node=node->next)
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
            if(curr->addr)
            {
                return &(curr->addr->data.data);
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
            tree_node *curr = root;
            for(size_t i=0;i<key_size;++i)
            {
                const uint8_t  code  = *(key_data++);
                bool           found = false;
                for(tree_node *node  = curr->chld.head;node;node=node->next)
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
            if(curr->addr)
            {
                data_node *d = curr->addr;
                curr->addr   = NULL;
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
        tree_node           *root;
        data_list            dlist;
        data_pool            dpool;

        inline void build_path( path &key, const data_node  *dnode) const
        {
            assert( NULL != dnode     );
            assert( dlist.owns(dnode) );
            key.free();
            const tree_node *curr = &(dnode->data.hook);
            while(curr->parent)
            {
                key.push_front(curr->code);
                curr=curr->parent;
            }
        }


        inline void release_data() throw()
        {
            data_node::destruct(dlist);
            data_node::destruct(dpool);
        }

    };

}

#endif

