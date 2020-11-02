
//! \file
#ifndef Y_PREFIX_TREE_INCLUDED
#define Y_PREFIX_TREE_INCLUDED 1

#include "y/associative/prefix/stem.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! a basic tree of data
    //
    //__________________________________________________________________________
    template <typename CODE, typename T>
    class prefix_tree
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                             //!< aliases
        class   data_node;                               //!< forward declaration
        typedef prefix_stem<CODE,data_node>   stem_type; //!< alias
        typedef typename stem_type::node_type node_type; //!< alias

        //______________________________________________________________________
        //
        //! internal data node
        //______________________________________________________________________
        class data_node
        {
        public:
            //! setup to functional node
            inline  data_node(const_type &args) : next(0), prev(0), hook(0), data(args) {}
            //! cleanup
            inline ~data_node() throw() { hook=0; }

            data_node *next; //!< for list/pool
            data_node *prev; //!< for list
            node_type *hook; //!< to internal leaf
            type       data; //!< actual data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_node);
        };
        typedef core::list_of<data_node> data_list; //!< alias
        typedef core::pool_of<data_node> data_pool; //!< alias


        //! setup
        inline explicit prefix_tree() : dl(), dp(), db()
        {
        }

        //! cleanup
        inline virtual ~prefix_tree() throw()
        {
            release();
        }

        //! insertion by path
        template <typename ITERATOR>
        bool insert(ITERATOR     curr,
                    const size_t size,
                    param_type  &args)
        {
            data_node *node = make_node(args);
            try {
                node_type *mark = 0;
                if(!db.grow(curr,size,node,&mark))
                {
                    kill_node(node);
                    return false;
                }
                else
                {
                    dl.push_back(node)->hook = mark;
                    assert(db.tell()==dl.size);
                    return true;
                }
            }
            catch(...)
            {
                kill_node(node);
                throw;
            }
        }

        //! insertion by sequence
        template <typename SEQUENCE>
        bool insert(SEQUENCE &seq,param_type &args)
        {
            return insert( seq.begin(), seq.size(), args);
        }

        //! search by path
        template <typename ITERATOR>
        const_type *search(ITERATOR curr, const size_t size) const throw()
        {
            const node_type *node = db.find(curr,size);
            if(node&&node->addr)
            {
                assert(node==node->addr->hook);
                return &(node->addr->data);
            }
            else
            {
                return NULL;
            }
        }

        //! search by sequence
        template <typename SEQUENCE>
        const_type *search(SEQUENCE &seq) const throw()
        {
            return search( seq.begin(), seq.size() );
        }

        //! search by path
        template <typename ITERATOR>
        type *search(ITERATOR curr, const size_t size) throw()
        {
            const prefix_tree &self = *this;
            return (type *) ( self.search(curr,size) );
        }

        //! search by sequence
        template <typename SEQUENCE>
        type *search(SEQUENCE &seq) throw()
        {
            return search( seq.begin(), seq.size() );
        }

        //! remove by path
        template <typename ITERATOR>
        bool remove(ITERATOR curr, const size_t size) throw()
        {
            const node_type *node = db.find(curr,size);
            if(node&&node->addr)
            {
                kill_node( dl.unlink(node->addr) );
                db.pull( (node_type*)node );
                assert(db.tell()==dl.size);
                return true;
            }
            else
            {
                return false;;
            }
        }

        //! remove by sequence
        template <typename SEQUENCE>
        bool remove(SEQUENCE &seq) throw()
        {
            return remove(seq.begin(),seq.size());
        }

        //! remove all extra memory
        inline void trim() throw()
        {
            db.cache_prune();
            empty_pool();
        }

        //! clean with memory keeping
        inline void free() throw()
        {
            db.reset();
            while(dl.size)
            {
                kill_node(dl.pop_back());
            }
        }

        //! release all possible memory
        inline void release() throw()
        {
            db.ditch();
            empty_pool(); assert(0==dp.size);
            while(dl.size)
            {
                data_node *node = dl.pop_back();
                self_destruct(*node);
                object::release1(node);
            }
            assert(0==dp.size);
            assert(0==dl.size);
            assert(0==db.tell());
        }


        //! number of items
        inline size_t size() const throw()
        {
            return dl.size;
        }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_tree);
        data_list dl; //!< list of hooked, live data nodes
        data_pool dp; //!< pool of zombie data nodes
        stem_type db; //!< the database

        void empty_pool() throw()
        {
            while(dp.size)
            {
                data_node *node = dp.query();
                object::release1(node);
            }
        }


        data_node *make_node(const_type &args)
        {
            data_node *node = dp.size ? dp.query() : object::acquire1<data_node>();
            try
            {
                return new (node) data_node(args);
            }
            catch(...)
            {
                dp.store(node);
                throw;
            }
        }

        void kill_node(data_node *node) throw()
        {
            assert(node); assert(0==node->next); assert(0==node->prev);
            self_destruct(*node);
            dp.store(node);
        }

    };

}

#endif
