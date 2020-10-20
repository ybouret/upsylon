
//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

#include "y/associative/affix/affix.hpp"
#include "y/iterate/linked.hpp"
#include "y/memory/buffers.hpp"
#include <cstring>

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

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! setup
        inline explicit affix_tree() : affix(), dl(), dp()
        {}
        
        //! cleanup
        inline virtual ~affix_tree() throw()
        {
            ditch();
        }

        //! copy
        inline affix_tree(const affix_tree &other) : affix(), dl(), dp()
        {
            memory::cppblock<uint8_t> blk( other.max_depth() );
            try
            {
                for(const data_node *node=other.dl.head;node;node=node->next)
                {

                    const tree_node *curr=node->hook;
                    const size_t     clen=curr->deep;
                    while(curr->parent)
                    {
                        blk[curr->deep] = curr->code;
                        curr=curr->parent;
                    }

                    if( !insert_at(*blk,clen,node->data) )
                    {
                        throw_multiple(node->hook);
                    }
                }

            }
            catch(...)
            {
                ditch();
                throw;
            }
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! maximum tree depth
        size_t max_depth() const throw()
        {
            size_t deep = 0;
            for(const data_node *node=dl.head;node;node=node->next)
            {
                const size_t  temp = node->hook->deep;
                if(temp>deep) deep = temp;
            }
            return deep;
        }


        //______________________________________________________________________
        //
        // insert
        //______________________________________________________________________

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

        //! insert using a block of memory
        inline bool insert_by(const void  *data,
                              const size_t size,
                              param_type   args)
        {
            assert(!(NULL==data&&size>0));
            return insert_at( static_cast<const char *>(data), size, args );
        }


        //! insert using text
        inline bool insert_by(const char  *text,
                              param_type   args)
        {
            return insert_by(text,text?strlen(text):0, args );
        }

        //! insert using text
        inline bool insert_by(const memory::ro_buffer &buff,
                              param_type               args)
        {
            return insert_by(buff.ro(),buff.length(),args);
        }


        //______________________________________________________________________
        //
        // search
        //______________________________________________________________________

        //! searching data along a path
        template <typename ITERATOR> inline
        type * search_at(ITERATOR     iter,
                         const size_t size) throw()
        {
            tree_node *node = (tree_node *)find(iter,size);
            if(node->addr)
            {
                return &( static_cast<data_node *>(node->addr)->data );
            }
            else
            {
                return NULL;
            }
        }



        //! search using a block of memory as path
        inline type * search_by(const void  *data,
                                const size_t size) throw()
        {
            assert(!(NULL==data&&size>0));
            return search_at( static_cast<const char *>(data), size);
        }



        //! search using a text as path
        inline type * search_by(const char *data) throw()
        {
            return search_by(data,data?strlen(data):0);
        }


        //! search using a buffer as path
        inline type * search_by(const memory::ro_buffer &buff) throw()
        {
            return search_by(buff.ro(), buff.length() );
        }


        //! searching along a const path
        template <typename ITERATOR> inline
        const_type * search_at(ITERATOR     iter,
                               const size_t size) const throw()
        {
            tree_node *node = (tree_node *)find(iter,size);
            if(node->addr)
            {
                return &( static_cast<data_node *>(node->addr)->data );
            }
            else
            {
                return NULL;
            }
        }

        //! search using a block of memory as path
        inline const_type * search_by(const void  *data,
                                      const size_t size) const throw()
        {
            assert(!(NULL==data&&size>0));
            return search_at( static_cast<const char *>(data), size);
        }



        //! search using a text as path
        inline const_type * search_by(const char *data) const throw()
        {
            return search_by(data,data?strlen(data):0);
        }


        //! search using a buffer as path
        inline const_type * search_by(const memory::ro_buffer &buff) const throw()
        {
            return search_by(buff.ro(), buff.length() );
        }


        //______________________________________________________________________
        //
        // remove
        //______________________________________________________________________
        //! inserting at a given path
        template <typename ITERATOR> inline
        bool remove_at(ITERATOR     iter,
                       const size_t size) throw()
        {
            tree_node *node = (tree_node *)find(iter,size);
            if(node->addr)
            {
                kill_data_node( dl.unlink( static_cast<data_node *>(node->addr)) );
                cut(node);
                return true;
            }
            else
            {
                return false;
            }
        }

        //! remove using a block of memory as path
        inline bool remove_by(const void  *data,
                              const size_t size) throw()
        {
            assert(!(NULL==data&&size>0));
            return remove_at( static_cast<const char *>(data), size);
        }



        //! remove using a text as path
        inline bool remove_by(const char *data) throw()
        {
            return remove_by(data,data?strlen(data):0);
        }


        //! remove using a buffer as path
        inline bool remove_by(const memory::ro_buffer &buff) throw()
        {
            return remove_by(buff.ro(), buff.length() );
        }


        //______________________________________________________________________
        //
        // memory management
        //______________________________________________________________________

        //! release excess memory
        inline void crop() throw()
        {
            prune();         // remove tree cache
            while(dp.size)   // remove data cache
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
                kill_data_node(dl.pop_back());
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

        //! destruct and store data_node
        inline void kill_data_node(data_node *node) throw()
        {
            assert(node);
            self_destruct(*node);
            node->hook = NULL;
            dp.store(node);
        }
        
        Y_DISABLE_ASSIGN(affix_tree);

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
