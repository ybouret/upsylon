//! \file
#ifndef Y_LIST_INCLUDED
#define Y_LIST_INCLUDED 1

#include "y/container/sequence.hpp"
//#include "y/sequence/addressable.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/iterate/linked.hpp"
#include "y/sort/merge.hpp"
#include "y/type/self-destruct.hpp"
#include "y/object.hpp"
#include <iostream>

namespace upsylon {

    //! linked list of nodes containing objects
    template <typename T>
    class list : public sequence<T>//, public addressable<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!<! aliases
        
        //! internal node type
        class node_type
        {
        public:
            //! build data by copy constructor
            inline  node_type(param_type args) : next(0), prev(0), data(args) {}

            //! build data by default constructor
            inline  node_type() : next(0), prev(0), data() {}

            //! destructor
            inline ~node_type() throw() {}

            node_type *next; //!< for list/pool
            node_type *prev; //!< for list
            T          data; //!< holds effective data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
        };

        typedef core::list_of<node_type> nodes_list; //!< hold active nodes
        typedef core::pool_of<node_type> nodes_pool; //!< hold inactive nodes

        //! default constructor
        inline explicit list() throw() : nodes(), cache() {}

        //! desructor
        inline virtual ~list() throw() { __release(); }

        //! with direction reservation of memory
        inline explicit list(const size_t n, const as_capacity_t &) :
        nodes(), cache()
        {
            try { __reserve(n); } catch(...) { __release(); throw; }
        }

        //! constructor with n objects having default constructor
        inline explicit list(const size_t n) throw() : nodes(), cache()
        {
            try {
                while( nodes.size < n )
                {
                    nodes.push_back( query() );
                }
            } catch(...) { __release(); throw; }
        }

        //! copy constructor
        list( const list &other ) : nodes(), cache()
        {
            try
            {
                for(const node_type *node = other.head; node; node=node->next )
                {
                    nodes.push_back( query(node->data) );
                }
            }
            catch(...)
            {
                release();
            }
        }

        //! assign
        list & operator=( const list &other )
        {
            if(this!=&other)
            {
                core::list_of<node_type> tmp;
                for(const node_type *node = other.head; node; node=node->next )
                {
                    tmp.push_back( query(node->data) );
                }
                this->free();
                tmp.swap_with(nodes);
            }
            return *this;
        }

        //! dynamic interface: size()
        inline virtual size_t size()     const throw() { return nodes.size; }
        //! dynamic inteface: capacity()
        inline virtual size_t capacity() const throw() { return nodes.size+cache.size; }

        //! container interface: free()
        inline virtual void free()    throw() { __free();        }
        //! container interface: release()
        inline virtual void release() throw() { __release(); }
        //! container interface: reserve()
        inline virtual void reserve(const size_t n) throw() { __reserve(n); }

        //! sequence interface: push_back()
        inline virtual void push_back( param_type args )
        {
            nodes.push_back( query(args) );
        }

        //! sequence interface: push_back()
        inline virtual void push_front( param_type args ) { nodes.push_front( query(args) ); }
        //! sequence interface: back()
        inline virtual type       & back() throw()       { assert(nodes.size>0); return nodes.tail->data; }
        //! sequence interface: back() const
        inline virtual const_type & back() const throw() { assert(nodes.size>0); return nodes.tail->data; }
        //! sequence interface : front()
        inline virtual type       & front() throw()       { assert(nodes.size>0); return nodes.head->data; }
        //! sequence interface : front(), const
        inline virtual const_type & front() const throw() { assert(nodes.size>0); return nodes.head->data; }
        //! sequence interface : pop_back()
        inline virtual void pop_back() throw()
        {
            assert(nodes.size>0);
            self_destruct( cache.store( nodes.pop_back() )->data );
        }
        //! sequence interface : pop_front()
        inline virtual void pop_front() throw()
        {
            assert(nodes.size>0);
            self_destruct( cache.store(nodes.pop_front())->data );
        }

        //! cluster interace: lower index=1
        inline virtual size_t lower_index() const throw() { return 1; }

        //! cluster interface: upper index=size()
        inline virtual size_t upper_index() const throw() { return this->size(); }

        //! cluster interface: SLOW access
        inline  virtual type       & operator[](const size_t i) throw()       { assert(i>0);assert(i<=size()); return nodes.fetch(i-1)->data; }

        //! cluster interface: SLOW access, CONST
        inline  virtual const_type & operator[](const size_t i) const throw() { assert(i>0);assert(i<=size()); return nodes.fetch(i-1)->data; }

        //! adjust size and pad if needed
        virtual void adjust( const size_t n, param_type pad )
        {
            while(nodes.size>n) pop_back();
            while(nodes.size<n) nodes.push_back( query(pad) );
        }

        typedef iterate::linked<type,node_type,iterate::forward>             iterator;        //!< forward iterator
        typedef iterate::linked<const_type,const node_type,iterate::forward> const_iterator;  //!< forward const iterator

        iterator begin() throw() { return iterator( nodes.head ); } //!< begin forward
        iterator end()   throw() { return iterator(0);            } //!< end forward

        const_iterator begin() const throw()   { return const_iterator( nodes.head ); } //!< begin forward const
        const_iterator end()   const throw()   { return const_iterator(0);            } //!< end forward const

        typedef iterate::linked<type,node_type,iterate::reverse>             reverse_iterator;        //!< reverse iterator
        typedef iterate::linked<const_type,const node_type,iterate::reverse> const_reverse_iterator;  //!< reverse const iterator

        reverse_iterator rbegin() throw() { return reverse_iterator( nodes.tail ); } //!< begin reverse
        reverse_iterator rend()   throw() { return reverse_iterator(0);            } //!< end reverse

        const_reverse_iterator rbegin() const throw() { return const_reverse_iterator( nodes.tail ); } //!< begin reverse const
        const_reverse_iterator rend()   const throw() { return const_reverse_iterator(0);            } //!< end reverse const

        //! output as a julia/octave array
        inline friend std::ostream & operator<<( std::ostream &os, const list &L )
        {
            os << '[';
            const node_type *curr = L.nodes.head;
            while(curr)
            {
                const node_type *next = curr->next;
                os << (curr->data);
                if(next)
                {
                    os << ' ';
                    curr = next;
                }
                else
                    break;
            }
            os << ']' << '\'';
            return os;
        }



        //! sorting by data
        template <typename FUNC>
        void sort(FUNC &func)
        {
            merging<node_type>::sort( nodes, compare_data<FUNC>, (void*) &func );
        }

        //! send low level control
        void __send_all( nodes_list &target )
        {
            target.merge_back(nodes);
        }

        //! recv from low_level
        void __recv_all( nodes_list &target )
        {
            nodes.merge_back(target);
        }

        //! reverse
        inline void reverse() throw()
        {
            nodes.reverse();
        }

        //! steal conten of other
        inline void merge_back( list &other ) throw()
        {
            if(this!=&other)
            {
                nodes.merge_back(other.nodes);
            }
        }

        //! delete cache
        inline void trim() throw()
        {
            while(cache.size>0)
            {
                node_type *node = cache.query();
                object::release1(node);
            }
        }

        //! remove_if, return number of removed
        template <typename IS_BAD>
        size_t remove_if( IS_BAD &is_bad ) throw()
        {
            size_t     nbad=0;
            {
                nodes_list temp;
                while( nodes.size )
                {
                    if(is_bad(nodes.head->data))
                    {
                        ++nbad;
                        pop_front();
                    }
                    else
                    {
                        temp.push_back( nodes.pop_front() );
                    }
                }
                nodes.swap_with(temp);
            }
            return nbad;
        }

    private:
        nodes_list nodes;
        nodes_pool cache;


        inline node_type *query_dead_node()
        {
            return (cache.size>0) ? cache.query() : object::acquire1<node_type>();
        }

        inline node_type *query(param_type args)
        {
            node_type *node = query_dead_node();
            try
            {
                new (node) node_type(args);
            }
            catch(...)
            {
                cache.store(node);
                throw;
            }
            return node;
        }

        inline node_type *query()
        {
            node_type *node = query_dead_node();
            try
            {
                new (node) node_type();
            }
            catch(...)
            {
                cache.store(node);
                throw;
            }
            return node;
        }



        inline void __reserve(size_t n)
        {
            while(n-->0) cache.store( object::acquire1<node_type>() );
        }

        inline void __free() throw()
        {
            while( nodes.size )
            {
                self_destruct( cache.store( nodes.pop_back() )->data );
            }
        }

        inline void __release() throw()
        {
            // remove active nodes
            while(nodes.size>0)
            {
                node_type *node = nodes.pop_back();
                self_destruct(node->data);
                object::release1(node);
            }
            // remove cached nodes
            trim();
        }

        template <typename FUNC> static inline
        int compare_data( const node_type *lhs, const node_type *rhs, void *args )
        {
            assert(args);
            FUNC &func = *(FUNC *)args;
            return func(lhs->data,rhs->data);
        }

    };
}

#endif

