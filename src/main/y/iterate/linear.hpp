//! \file
#ifndef Y_ITERATOR_LINEAR
#define Y_ITERATOR_LINEAR 1

#include "y/iterate/iterate.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace iterate
    {

        //! linked iterator
        template <
        typename  T,
        direction D>
        class linear
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases

            //! initialize from address
            inline linear(T *p) throw() : addr( (mutable_type *)p ) {}
            //! copy
            inline linear(const linear &other) throw() : addr(other.addr) {}
            //! assign
            inline linear & operator=(const linear &other) throw()
            {
                addr = other.addr;
                return *this;
            }

            //! check different
            inline friend bool operator!=( const linear &lhs, const linear &rhs ) throw()
            {
                return lhs.addr != rhs.addr;
            }

            //! pre-increment
            inline linear & operator++() throw()
            {
                increase( int2type<D>() );
                return *this;
            }

            //! post-increment
            inline linear operator++(int) throw()
            {
                const linear tmp(*this);
                ++(*this);
                return tmp;
            }

            //! pre-decrement
            inline linear & operator--() throw()
            {
                decrease(int2type<D>());
                return *this;
            }

            //! post-decrement
            inline linear operator--(int) throw()
            {
                const linear tmp(*this);
                --(*this);
                return tmp;
            }

            //! in place +
            inline linear & operator+=(const ptrdiff_t n) throw()
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
            inline linear & operator-=(const ptrdiff_t n) throw()
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
            inline linear operator+(const ptrdiff_t n) const throw()
            {
                linear tmp(*this);
                return tmp += n;
            }

            //! -
            inline linear operator-(const ptrdiff_t n) const throw()
            {
                linear tmp(*this);
                return tmp -= n;
            }

            inline type & operator*() throw()
            {
                assert(addr);
                return *addr;
            }

            inline const_type & operator*() const throw()
            {
                assert(addr);
                return *addr;
            }

        private:
            mutable_type *addr;
            void increase( int2type<forward> ) throw() { ++addr; }
            void decrease( int2type<forward> ) throw() { --addr; }
            void increase( int2type<reverse> ) throw() { --addr; }
            void decrease( int2type<reverse> ) throw() { ++addr; }
        };

    }
}

#endif
