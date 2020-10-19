
//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

#include "y/associative/affix/affix.hpp"

namespace upsylon
{
    
    //! affix tree
    template <typename T>
    class affix_tree
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                 //!< aliases
        typedef affix::tree_node tree_node;  //!< alias
        
        //______________________________________________________________________
        //
        //! base class
        //______________________________________________________________________
        class data_node
        {
        public:
            //! setup
            inline data_node(param_type args) :
            next(0),
            prev(0),
            hook(0),
            data(args)
            {
            }
            
            //! cleanup
            inline ~data_node() throw() {}
            
            data_node *next; //!< for list
            data_node *prev; //!< for list
            tree_node *hook; //!< for linking
            type       data; //!< effective data
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_node);
        };
        
        typedef core::list_of<data_node> data_list; //!< alias
        typedef core::pool_of<data_node> data_pool; //!< alias
        
        //! setup
        inline explicit affix_tree() : dl(), dp(), db()
        {}
        
        //! cleanup
        inline virtual ~affix_tree() throw()
        {
            release_();
        }
        
        //! inserting at a given path
        template <typename ITERATOR>
        bool insert_at(ITERATOR     iter,
                       const size_t size,
                       param_type   args)
        {
            data_node *dnode = make_data_node(args);
            try
            {
                tree_node *tnode = db.insert_at_path(iter,size,dnode);
                if(!tnode)
                {
                    kill_data_node(dnode);
                    return false;
                }
                else
                {
                    dl.push_back(dnode);
                    dnode->hook = tnode;
                    return true;
                }
            }
            catch(...)
            {
                kill_data_node(dnode);
                throw;
            }
        }
        
        //! remove excess data node
        inline void trim() throw()
        {
            db.gc(0);
            while(dp.size)
            {
                data_node *node = dp.query();
                object::release1(node);
            }
        }
        
        //! remove registered data, keep all memory
        inline void free() throw()
        {
            db.clear();
            while(dl.size)
            {
                delete_data_node(dl.pop_back());
            }
        }
        
        //! release all possible memory
        inline void release() throw()
        {
            release_();
        }
        
    private:
        data_list   dl; //!< data list
        data_pool   dp; //!< data pool
        affix       db; //!< data base
        
        inline void release_() throw()
        {
            db.clear();
            db.gc(0);
            trim();
            while(dl.size)
            {
                data_node *node = dl.pop_back();
                self_destruct(*node);
                object::release1(node);
            }
        }
        
        //! return a constructed data_node
        inline data_node  *make_data_node(const_type &args)
        {
            data_node *node = (dp.size>0) ? dp.query() : object::acquire1<data_node>();
            try {
                return new (node) data_node(args);
            } catch(...) {
                dp.store(node);
                throw;
            }
        }
        
        inline void kill_data_node(data_node *node) throw()
        {
            assert(node);
            self_destruct(*node);
            node->hook = NULL;
            dp.store(node);
        }
        
        Y_DISABLE_COPY_AND_ASSIGN(affix_tree);
    };
    
}

#endif
