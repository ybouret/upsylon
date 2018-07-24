//! \file
#ifndef Y_LIST_INCLUDED
#define Y_LIST_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/iterate/linked.hpp"
#include <iostream>

namespace upsylon
{
    //! linked list of nodes containing objects
    template <typename T>
    class list : public sequence<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!<! aliases
        
        //! internal node type
        class node_type
        {
        public:
            //! build data by copy constructor
            inline  node_type(param_type args) : next(0), prev(0), data(args) {}
            //! destructor
            inline ~node_type() throw() {}

            node_type *next; //!< for list/pool
            node_type *prev; //!< for list
            T          data; //!< holds effective data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(node_type);
        };

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
        inline virtual void push_front( param_type args )
        {
            nodes.push_front( query(args) );
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

    private:
        core::list_of<node_type> nodes;
        core::pool_of<node_type> cache;
        inline node_type *query(param_type args)
        {
            node_type *node = (cache.size>0) ? cache.query() : object::acquire1<node_type>();
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

        inline void __reserve(size_t n)
        {
            while(n-->0) cache.store( object::acquire1<node_type>() );
        }

        inline void __free() throw()
        {
            while( nodes.size )
            {
                node_type *node = nodes.pop_back();
                destruct(node);
                cache.store(node);
            }
        }

        inline void __release() throw()
        {
            while( nodes.size )
            {
                node_type *node = nodes.pop_back();
                destruct(node);
                object::release1(node);
            }
            while( cache.size )
            {
                node_type *node = cache.query();
                object::release1(node);
            }
        }
    };
}

#endif

