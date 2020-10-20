
//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

#include "y/associative/affix/affix.hpp"
#include "y/iterate/linked.hpp"

namespace upsylon
{
    
    //! affix tree
    template <typename T>
    class affix_tree : public affix
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                 //!< aliases

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
        inline explicit affix_tree() : affix(), dl(), dp()
        {}
        
        //! cleanup
        inline virtual ~affix_tree() throw()
        {
            ditch();
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
                tree_node *tnode = grow(iter,size,dnode);
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
        
        //! remove excess memory
        inline void crop() throw()
        {
            prune();
            while(dp.size)
            {
                data_node *node = dp.query();
                object::release1(node);
            }
        }
        
        //! remove registered data, keep all memory
        inline void erase() throw()
        {
            reset();
            while(dl.size)
            {
                delete_data_node(dl.pop_back());
            }
        }
        
        //! release all possible memory
        inline void ditch() throw()
        {
            reset();
            crop();
            while(dl.size)
            {
                data_node *node = dl.pop_back();
                self_destruct(*node);
                object::release1(node);
            }
        }
        
    private:
        data_list   dl; //!< data list
        data_pool   dp; //!< data pool

        
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

    public:
        typedef iterate::linked<type,data_node,iterate::forward >            iterator;        //!< forward iterator
        typedef iterate::linked<const_type,const data_node,iterate::forward> const_iterator;  //!< forward iterator

        inline iterator       begin() throw() { return iterator(dl.head); } //!< begin
        inline iterator       end()   throw() { return iterator(NULL);       } //!< end

        inline const_iterator begin() const throw() { return const_iterator(dl.head); } //!< begin, const
        inline const_iterator end()   const throw() { return const_iterator(NULL);       } //!< end, const
    };
    
}

#endif
