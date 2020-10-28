
//! \file
#ifndef Y_AFFIX_TREE_INCLUDED
#define Y_AFFIX_TREE_INCLUDED 1

#include "y/associative/affix/affix.hpp"
#include "y/iterate/linked.hpp"
#include "y/memory/buffers.hpp"
#include "y/sort/merge.hpp"
#include "y/type/self-destruct.hpp"
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
            
            data_node       *next; //!< for list
            data_node       *prev; //!< for list
            const tree_node *hook; //!< for linking
            type             data; //!< effective data
            
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

        //! setup with some memory
        inline explicit affix_tree(const size_t n, const as_capacity_t &) :
        affix(), dl(), dp()
        {
            gain(n);
        }

        //! cleanup
        inline virtual ~affix_tree() throw()
        {
            ditch();
        }

        //! copy
        inline affix_tree(const affix_tree &other) : affix(), dl(), dp()
        {
            memory::cppblock<uint8_t> path( other.max_depth() );
            try
            {
                for(const data_node *node=other.dl.head;node;node=node->next)
                {
                    const tree_node *curr=node->hook;
                    curr->encode(path);
                    if( !insert_at(*path,curr->deep,node->data,NULL) )
                    {
                        throw_multiple(curr);
                    }
                }
            }
            catch(...)
            {
                ditch();
                throw;
            }
        }

        //! assign
        affix_tree & operator=( const affix_tree & other )
        {
            if(this!=&other)
            {
                affix_tree temp(other);
                swap_with(temp);
            }
            return *this;
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
                       param_type   args,
                       tree_mark   *mark)
        {
            data_node *dnode = make_data_node(args);
            try
            {
                tree_node *tnode = grow(iter,size,dnode,mark);
                if(!tnode)
                {
                    kill_data_node(dnode);
                    assert(dl.size==tell());
                    return false;
                }
                else
                {
                    dl.push_back(dnode);
                    dnode->hook = tnode;
                    assert(dl.size==tell());
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
                              param_type   args,
                              tree_mark   *mark=0)
        {
            assert(!(NULL==data&&size>0));
            return insert_at( static_cast<const char *>(data), size, args, mark);
        }


        //! insert using text
        inline bool insert_by(const char  *text,
                              param_type   args,
                              tree_mark   *mark=0)
        {
            return insert_by(text,text?strlen(text):0, args, mark);
        }

        //! insert using text
        inline bool insert_by(const memory::ro_buffer &buff,
                              param_type               args,
                              tree_mark               *mark=0)
        {
            return insert_by(buff.ro(),buff.length(),args,mark);
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
            if(node&&node->addr)
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
            if(node&&node->addr)
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
            if(node && node->addr)
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

        //! gain excess memory
        inline void gain(size_t n)
        {
            while(n-- > 0)
            {
                dp.store( object::acquire1<data_node>() );
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

        //! sort content
        template <typename FUNC>
        void sort_with(FUNC &func)
        {
            merging<data_node>::sort(dl,call_compare<FUNC>,(void*)&func);
        }

        //! no throw swap
        inline void swap_with(affix_tree &other) throw()
        {
            xch(other);             // affix part
            dl.swap_with(other.dl); // data part 1/2
            dp.swap_with(other.dp); // data part 2/2
        }

    protected:
        //! manipulate data for derived class
        const data_node * head()    const throw() { return dl.head; }
        size_t            dl_size() const throw() { return dl.size; }
        size_t            dp_size() const throw() { return dp.size; }

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

        template <typename FUNC> static inline
        int call_compare(const data_node *lhs, const data_node *rhs, void *args )
        {
            assert(lhs); assert(rhs); assert(args);
            FUNC &func  = *(FUNC *)args;
            return func(lhs->data,rhs->data);
        }


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
