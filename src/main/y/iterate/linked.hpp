//! \file
#ifndef Y_ITERATOR_LINKED
#define Y_ITERATOR_LINKED 1

#include "y/iterate/iterate.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace iterate
    {

        //! linked iterator
        template <
        typename  T,
        typename  NODE,
        direction D>
        class linked
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
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

            //! content
            inline type & operator*() throw()
            {
                assert(node); return node->data;
            }

            //! const content
            inline const_type & operator*() const throw()
            {
                assert(node); return node->data;
            }

            //! transitive
            inline type * operator->() throw()
            {
                assert(node); return &(node->data);
            }

            //! transitive const
            inline const_type * operator->() const throw()
            {
                assert(node); return &(node->data);
            }


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

