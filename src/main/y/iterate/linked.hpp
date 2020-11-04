//! \file
#ifndef Y_ITERATOR_LINKED
#define Y_ITERATOR_LINKED 1

#include "y/iterate/iterate.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace iterate
    {

        //! default access to node data
        template <typename T,typename NODE>
        struct linked_data
        {
            Y_DECL_ARGS(T,type); //!< aliases
            //! node->data
            inline static const_type & get(const NODE *node) throw() { assert(node); return node->data; }
        };

        //______________________________________________________________________
        //
        //! linked iterator
        //______________________________________________________________________
        template <
        typename  T,
        typename  NODE,
        direction D,
        typename  access = linked_data<T,NODE> >
        class linked
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                                              //!< aliases
            typedef NODE                                     node_type;       //!< alias
            typedef typename type_traits<NODE>::mutable_type mutable_node;    //!< alias
            typedef const mutable_node                       const_node;      //!< alias


            //! ctor
            inline  linked(NODE *n) throw() : node(n) {}
            //! dtor
            inline ~linked() throw() {}
            //! copy
            inline  linked(const linked &other) throw() : node(other.node) {}
            //! assign
            inline  linked &operator=(const linked &other) throw()
            {
                node = other.node;
                return *this;
            }

            //! pre-increment
            inline linked & operator++() throw()
            {
                increase( int2type<D>() );
                return *this;
            }

            //! post-increment
            inline linked operator++(int) throw()
            {
                const linked tmp(*this);
                ++(*this);
                return tmp;
            }

            //! pre-decrement
            inline linked & operator--() throw()
            {
                decrease(int2type<D>());
                return *this;
            }

            //! post-decrement
            inline linked operator--(int) throw()
            {
                const linked tmp(*this);
                --(*this);
                return tmp;
            }

            //! in place +
            inline linked & operator+=(const ptrdiff_t n) throw()
            {
                if(n>0)
                {
                    for(ptrdiff_t i=n;i>0;--i)
                    {
                        increase( int2type<D>() );
                    }
                }
                else if(n<0)
                {
                    for(ptrdiff_t i=-n;i>0;--i)
                    {
                        decrease( int2type<D>() );
                    }
                }
                return *this;
            }

            //! in place -
            inline linked & operator-=(const ptrdiff_t n) throw()
            {
                if(n>0)
                {
                    for(ptrdiff_t i=n;i>0;--i)
                    {
                        decrease( int2type<D>() );
                    }
                }
                else if(n<0)
                {
                    for(ptrdiff_t i=-n;i>0;--i)
                    {
                        increase( int2type<D>() );
                    }
                }
                return *this;
            }

            //! +
            inline linked operator+(const ptrdiff_t n) const throw()
            {
                linked tmp(*this);
                return tmp += n;
            }

            //! -
            inline linked operator-(const ptrdiff_t n) const throw()
            {
                linked tmp(*this);
                return tmp -= n;
            }

            //! testing for loops
            inline friend bool operator!=( const linked &lhs, const linked &rhs ) throw()
            {
                return lhs.node != rhs.node;
            }

            //! testing for loops
            inline friend bool operator==( const linked &lhs, const linked &rhs ) throw()
            {
                return lhs.node == rhs.node;
            }

            //! content
            inline type & operator*() throw()
            {
                assert(node); return (type&) access::get(node);
            }

            //! const content
            inline const_type & operator*() const throw()
            {
                assert(node); return access::get(node);
            }

            //! transitive
            inline type * operator->() throw()
            {
                assert(node); return (type *) & access::get(node);
            }

            //! transitive const
            inline const_type * operator->() const throw()
            {
                assert(node); return & access::get(node);
            }

            //! get node
            inline node_type  & get() throw() { assert(node); return *node; }

            //! get node, const
            inline const_node & get() const throw() { assert(node); return *node; }

        private:
            NODE *node;
            inline void increase( int2type<forward> ) throw()
            {
                assert(node); node=node->next;
            }
            inline void decrease( int2type<forward> ) throw()
            {
                assert(node); node=node->prev;
            }
            inline void increase( int2type<reverse> ) throw()
            {
                assert(node); node=node->prev;
            }
            inline void decrease( int2type<reverse> ) throw()
            {
                assert(node); node=node->next;
            }
        };

    }
}

#endif

