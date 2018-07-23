//! \file
#ifndef Y_ITERATOR_META
#define Y_ITERATOR_META 1

#include "y/iterate/iterate.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace iterate
    {

        //! meta iterator
        template <
        typename  KEY,
        typename  T,
        typename  NODE,
        direction D>
        class meta
        {
        public:
            Y_DECL_ARGS(T,type);         //!< aliases
            Y_DECL_ARGS(KEY,key_type);   //!< aliases

            //! ctor
            inline  meta(NODE *n) throw() : node(n) {}
            //! dtor
            inline ~meta() throw() {}
            //! copy
            inline  meta(const meta &other) throw() : node(other.node) {}
            //! assign
            inline  meta &operator=(const meta &other) throw()
            {
                node = other.node;
                return *this;
            }

            //! pre-increment
            inline meta & operator++() throw()
            {
                increase( int2type<D>() );
                return *this;
            }

            //! post-increment
            inline meta operator++(int) throw()
            {
                const meta tmp(*this);
                ++(*this);
                return tmp;
            }

            //! pre-decrement
            inline meta & operator--() throw()
            {
                decrease(int2type<D>());
                return *this;
            }

            //! post-decrement
            inline meta operator--(int) throw()
            {
                const meta tmp(*this);
                --(*this);
                return tmp;
            }

            //! in place +
            inline meta & operator+=(const ptrdiff_t n) throw()
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
            inline meta & operator-=(const ptrdiff_t n) throw()
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
            inline meta operator+(const ptrdiff_t n) const throw()
            {
                meta tmp(*this);
                return tmp += n;
            }

            //! -
            inline meta operator-(const ptrdiff_t n) const throw()
            {
                meta tmp(*this);
                return tmp -= n;
            }

            //! testing for loops
            inline friend bool operator!=( const meta &lhs, const meta &rhs ) throw()
            {
                return lhs.node != rhs.node;
            }

            //! content
            inline type & operator*() throw()
            {
                assert(node);
                assert(node->addr);
                return node->addr->data;
            }

            //! const content
            inline const_type & operator*() const throw()
            {
                assert(node);
                assert(node->addr);
                return node->addr->data;
            }

            //! to access node
            inline type * operator->() throw()
            {
                assert(node); assert(node->addr); return &(node->addr->data);
            }

            //! transitive const
            inline const_type * operator->() const throw()
            {
                assert(node); assert(node->addr); return &(node->addr->data);
            }

            inline const_key_type & key() const throw()
            {
                assert(node); assert(node->addr); return node->addr->key();
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

