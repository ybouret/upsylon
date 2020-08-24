

//! \file
#ifndef Y_AP_PRIME_ITERATOR_INCLUDED
#define Y_AP_PRIME_ITERATOR_INCLUDED 1

#include "y/yap/library.hpp"

namespace upsylon
{

    namespace yap
    {

        class prime_iterator
        {
        public:
            typedef number::utype utype;

            prime_iterator( library & ) throw();
            ~prime_iterator() throw();
            prime_iterator(const prime_iterator &) throw();
            prime_iterator & operator=(const prime_iterator &) throw();

            const prime & operator*()  const throw();
            const prime * operator->() const throw();

            prime_iterator & operator++();      //!< prefix++ operator
            prime_iterator   operator++(int);   //!< postfix++ operator
            friend prime_iterator operator+(const size_t,const prime_iterator &);
            friend prime_iterator operator+(const prime_iterator &,const size_t);

            inline friend bool operator==(const prime_iterator &lhs, const prime_iterator &rhs) throw() { return lhs.ptr==rhs.ptr; }
            inline friend bool operator==(const prime_iterator &lhs, const natural &rhs)        throw() { return eq(lhs,rhs); }
            inline friend bool operator==(const natural &lhs, const prime_iterator &rhs)        throw() { return eq(rhs,lhs); }
            inline friend bool operator==(const prime_iterator &lhs, const utype rhs)           throw() { return eq(lhs,rhs); }
            inline friend bool operator==(const utype lhs, const prime_iterator &rhs)           throw() { return eq(rhs,lhs); }

            inline friend bool operator!=(const prime_iterator &lhs, const prime_iterator &rhs) throw() { return lhs.ptr!=rhs.ptr; }
            inline friend bool operator!=(const prime_iterator &lhs, const natural &rhs)        throw() { return neq(lhs,rhs); }
            inline friend bool operator!=(const natural &lhs, const prime_iterator &rhs)        throw() { return neq(rhs,lhs); }
            inline friend bool operator!=(const prime_iterator &lhs, const utype rhs)           throw() { return neq(lhs,rhs); }
            inline friend bool operator!=(const utype lhs, const prime_iterator &rhs)           throw() { return neq(rhs,lhs); }



        private:
            const prime   *ptr;
            library       &lib;
            void  next();
            void  next(size_t n);
            template <typename T>
            static inline bool eq(const prime_iterator &lhs, const T &rhs) throw()
            {
                const natural &n = *lhs;
                return n == rhs;
            }

            template <typename T>
            static inline bool neq(const prime_iterator &lhs, const T &rhs) throw()
            {
                const natural &n = *lhs;
                return n != rhs;
            }


        };

    }

}

#endif

