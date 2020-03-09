
//! \file

#ifndef Y_SUFFIX_TREE_INCLUDED
#define Y_SUFFIX_TREE_INCLUDED 1

#include "y/core/knode.hpp"
#include "y/ios/ostream.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/code/utils.hpp"

namespace upsylon {

    namespace core {
        
    }
    
    //! generic suffix tree
    template <typename T>
    class suffix_tree
    {
    public:
        Y_DECL_ARGS(T,type);                        //!< aliases
        typedef core::knode<T>           data_node; //!< data node
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

        //! accessing root
        inline const tree_node & get_root() const throw() { return *root; }

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
                curr->addr = dlist.push_back( data_node::create_alive_with(dpool,args) );
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
                return &(curr->addr->data);
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

        inline void trim() throw()
        {
            
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(suffix_tree);
        tree_node           *root;
        data_list            dlist;
        data_pool            dpool;



        void release_data() throw()
        {
            data_node::destruct(dlist);
            data_node::destruct(dpool);
        }

    };

}

#endif

