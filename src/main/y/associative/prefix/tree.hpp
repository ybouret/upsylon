
//! \file
#ifndef Y_PREFIX_TREE_INCLUDED
#define Y_PREFIX_TREE_INCLUDED 1

#include "y/associative/prefix/stem.hpp"
#include "y/container/container.hpp"
#include "y/memory/buffers.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! a basic tree of data
    //
    //__________________________________________________________________________
    template <typename CODE, typename T>
    class prefix_tree : public container
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

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________

        //! setup
        inline explicit prefix_tree() : mk(0), dl(), dp(), db()
        {
        }

        //! cleanup
        inline virtual ~prefix_tree() throw()
        {
            release();
        }

        //! copy
        inline prefix_tree(const prefix_tree &other) :
        collection(),
        container(),
        mk(0),
        dl(),
        dp(),
        db()
        {
            memory::cppblock<CODE> path(other.max_depth());
            for(const data_node *node=other.dl.head;node;node=node->next)
            {
                const size_t depth = node->hook->encode(path);
                if(!insert(*path,depth,node->data))
                {
                    prefix_::throw_unexpected_multiple_path();
                }
            }
        }

        //! assign
        inline prefix_tree & operator=(const prefix_tree &other)
        {
            prefix_tree temp(other);
            swap_with(temp);
            return *this;
        }


        //----------------------------------------------------------------------
        //
        // insertion API
        //
        //----------------------------------------------------------------------

        //! insertion by path
        template <typename ITERATOR>
        inline bool insert(ITERATOR     curr,
                    const size_t size,
                    const_type  &args)
        {
            mk              = 0;
            data_node *node = make_node(args);
            try {
                if(!db.grow(curr,size,node,&mk))
                {
                    kill_node(node);
                    return false;
                }
                else
                {
                    dl.push_back(node)->hook = mk;
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
        inline bool insert(SEQUENCE   &seq,
                    param_type &args)
        {
            return insert(seq.begin(),seq.size(),args);
        }

        //! insert by C-style array
        inline bool insert(const CODE *code, param_type args)
        {
            return insert(code,stem_type::codelen(code),args);
        }


        //----------------------------------------------------------------------
        //
        // const search API
        //
        //----------------------------------------------------------------------

        //! search by path
        template <typename ITERATOR>
        inline const_type *search(ITERATOR curr, const size_t size) const throw()
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
        inline const_type *search(SEQUENCE &seq) const throw()
        {
            return search( seq.begin(), seq.size() );
        }

        //! search by C-style array
        template <typename SEQUENCE>
        inline const_type *search(const CODE *code) const throw()
        {
            return search(code,stem_type::codelen(code));
        }

        //----------------------------------------------------------------------
        //
        // mutable search API
        //
        //----------------------------------------------------------------------

        //! search by path
        template <typename ITERATOR>
        inline type *search(ITERATOR curr, const size_t size) throw()
        {
            const prefix_tree &self = *this;
            return (type *) ( self.search(curr,size) );
        }

        //! search by sequence
        template <typename SEQUENCE>
        inline type *search(SEQUENCE &seq) throw()
        {
            return search( seq.begin(), seq.size() );
        }

        //! search by C-style
        inline type *search(const CODE *code) throw()
        {
            return search(code,stem_type::codelen(code));
        }

        //----------------------------------------------------------------------
        //
        // remove API
        //
        //----------------------------------------------------------------------

        //! remove by path
        template <typename ITERATOR>
        inline bool remove(ITERATOR curr, const size_t size) throw()
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
                return false;
            }
        }

        //! remove by sequence
        template <typename SEQUENCE>
        inline bool remove(SEQUENCE &seq) throw()
        {
            return remove(seq.begin(),seq.size());
        }

        //! remove by C-style array
        inline bool remove(const CODE *code) throw()
        {
            return remove(code,stem_type::codelen(code));
        }

        //----------------------------------------------------------------------
        //
        // other
        //
        //----------------------------------------------------------------------

        //! remove all extra memory
        inline void trim() throw()
        {
            db.cache_prune();
            empty_pool();
        }



        //! clean with memory keeping
        inline virtual void free() throw()
        {
            db.reset();
            while(dl.size)
            {
                kill_node(dl.pop_back());
            }
        }

        //! release all possible memory
        inline virtual void release() throw()
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
        inline virtual size_t size() const throw()
        {
            return dl.size;
        }

        //! capacity
        inline virtual size_t capacity() const throw()
        {
            return dl.size+dp.size;
        }

        //! reserve extra data nodes
        inline virtual void reserve(size_t n)
        {
            while(n-- > 0) dp.store( object::acquire1<data_node>() );
        }


        //! get root to draw graph
        const ios::vizible &get_root() const throw()
        {
            return db.get_root();
        }

        //! sort data
        template <typename FUNC>
        inline void sort(FUNC &func)
        {
            merging<data_node>::sort(dl,call<FUNC>, (void *) &func);
        }

        //! helper to iteratre
        inline const data_node *head() const throw()
        {
            return dl.head;
        }

        //! get max depth
        inline size_t max_depth() const throw()
        {
            size_t ans = 0;
            for(const data_node *node=dl.head;node;node=node->next)
            {
                const size_t tmp = node->hook->depth;
                if(tmp>ans)  ans = tmp;
            }
            return ans;
        }

        //! no-throw swap
        inline void swap_with(prefix_tree &other) throw()
        {
            cswap(mk,other.mk);
            dl.swap_with(other.dl);
            dp.swap_with(other.dp);
            db.xch(other.db);
        }

    private:
        node_type *mk;  //!< last mark on insert
        data_list  dl;  //!< list of hooked, live data nodes
        data_pool  dp;  //!< pool of zombie data nodes
        stem_type  db;  //!< the database

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

        template <typename FUNC>
        static inline int call(const data_node *lhs, const data_node *rhs, void *args) throw()
        {
            assert(args);
            FUNC &func = *(FUNC *)args;
            return func(lhs->data,rhs->data);
        }

    };

}

#endif
