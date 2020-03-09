//! \file
#ifndef Y_LIST_INCLUDED
#define Y_LIST_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/core/knode.hpp"
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
        
        typedef core::knode<T>                node_type;
        typedef typename node_type::list_type nodes_list;
        typedef typename node_type::pool_type nodes_pool;

        
        //! default constructor
        inline explicit list() throw() : nodes(), cache() {}

        //! desructor
        inline virtual ~list() throw() { __release(); }

        //! with direction reservation of memory
        inline explicit list(const size_t n, const as_capacity_t &) :
        nodes(), cache()
        {
            try        { node_type::prefetch(cache,n);      }
            catch(...) { node_type::destruct(cache); throw; }
        }

        //! constructor with n objects having default constructor
        inline explicit list(const size_t n)  : nodes(), cache()
        {
            try
            {
                while( nodes.size < n )
                {
                    nodes.push_back( node_type::create() );
                }
            } catch(...) { node_type::destruct(nodes); throw; }
        }

        //! constructor with n objects having copy constructor
        inline explicit list(const size_t n, param_type value)  : nodes(), cache()
        {
            try {
                while( nodes.size < n )
                {
                    nodes.push_back( node_type::create_alive(value) );
                }
            } catch(...) { node_type::destruct(nodes); throw; }
        }

        //! copy constructor
        list( const list &other ) : nodes(), cache()
        {
            try
            {
                for(const node_type *node = other.nodes.head; node; node=node->next )
                {
                    nodes.push_back( node_type::create_alive(node->data) );
                }
            }
            catch(...)
            {
                assert(0==cache.size);
                node_type::destruct(nodes);
                throw;
            }
        }

        //! assign
        list & operator=( const list &other )
        {
            if(this!=&other)
            {
                core::list_of<node_type> tmp;
                for(const node_type *node = other.nodes.head; node; node=node->next )
                {
                    tmp.push_back( node_type::create_alive(node->data) );
                }
                __free();
                tmp.swap_with(nodes);
            }
            return *this;
        }

        //! dynamic interface: size()
        inline virtual size_t size()     const throw() { return nodes.size; }
        //! dynamic inteface: capacity()
        inline virtual size_t capacity() const throw() { return nodes.size+cache.size; }

        //! container interface: free()
        inline virtual void free()    throw() {  node_type::destruct_to(cache,nodes); }
        
        //! container interface: release()
        inline virtual void release() throw() { __release(); }
        
        //! container interface: reserve()
        inline virtual void reserve(const size_t n) throw() { node_type::prefetch(cache,n); }

        //! sequence interface: push_back()
        inline virtual void push_back(param_type args)
        {
            nodes.push_back( node_type::create_alive_with(args,cache) );
        }

        //! sequence interface: push_back()
        inline virtual void push_front(param_type args)
        {
            nodes.push_front( node_type::create_alive_with(args,cache) );
        }

        //! specific: is enough memory
        inline void push_back_(param_type args)
        {
            nodes.push_back( node_type::create_alive_with_(args,cache) );
        }

        //! specific: is enough memory
        inline void push_front_(param_type args)
        {
            nodes.push_front( node_type::create_alive_with_(args,cache) );
        }
        
        //! specific
        inline void guarantee(const size_t n)
        {
            while(cache.size<n) cache.store( object::acquire1<node_type>() );
        }

        //! sequence interface: back()
        inline virtual type       & back() throw()        { assert(nodes.size>0); return nodes.tail->data; }
        //! sequence interface: back() const
        inline virtual const_type & back() const throw()  { assert(nodes.size>0); return nodes.tail->data; }
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


        //! addressable interface: SLOW access
        inline  virtual type       & operator[](const size_t i) throw()       { assert(i>0);assert(i<=size()); return nodes.fetch(i-1)->data; }

        //! accessible interface: SLOW access, CONST
        inline  virtual const_type & operator[](const size_t i) const throw() { assert(i>0);assert(i<=size()); return nodes.fetch(i-1)->data; }

        //! adjust size and pad if needed
        virtual void adjust( const size_t n, param_type pad )
        {
            while(nodes.size>n) pop_back();
            while(nodes.size<n) nodes.push_back( node_type::create_alive_with(pad,cache) );
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
        inline void trim(const size_t nmax=0) throw()
        {
            while(cache.size>nmax)
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
        

        inline void __free() throw()
        {
            node_type::destruct_to(cache,nodes);
        }

        inline void __release() throw()
        {
            node_type::destruct(nodes);
            node_type::destruct(cache);
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

