

//! \file
#ifndef Y_AP_PRIME_ITERATOR_INCLUDED
#define Y_AP_PRIME_ITERATOR_INCLUDED 1

#include "y/yap/library.hpp"

namespace upsylon
{

    namespace yap
    {
        //______________________________________________________________________
        //
        //
        //! sequential primes
        //
        //______________________________________________________________________
        class prime_iterator
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef number::utype utype; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            prime_iterator( library & ) throw();                        //!< setup from 2
            ~prime_iterator() throw();                                  //!< cleanup
            prime_iterator(const prime_iterator &) throw();             //!< copy
            prime_iterator & operator=(const prime_iterator &) throw(); //!< assign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const prime & operator*()  const throw(); //!< content
            const prime * operator->() const throw(); //!< transitive access

            //__________________________________________________________________
            //
            // moving
            //__________________________________________________________________
            prime_iterator & operator++();      //!< prefix++ operator
            prime_iterator   operator++(int);   //!< postfix++ operator
            friend prime_iterator operator+(const size_t,const prime_iterator &); //!< take steps
            friend prime_iterator operator+(const prime_iterator &,const size_t); //!< take steps

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________
            inline friend bool operator==(const prime_iterator &lhs, const prime_iterator &rhs) throw() { return lhs.ptr==rhs.ptr; } //!< ==
            inline friend bool operator==(const prime_iterator &lhs, const natural &rhs)        throw() { return eq(lhs,rhs); }      //!< ==
            inline friend bool operator==(const natural &lhs, const prime_iterator &rhs)        throw() { return eq(rhs,lhs); }      //!< ==
            inline friend bool operator==(const prime_iterator &lhs, const utype rhs)           throw() { return eq(lhs,rhs); }      //!< ==
            inline friend bool operator==(const utype lhs, const prime_iterator &rhs)           throw() { return eq(rhs,lhs); }      //!< ==

            inline friend bool operator!=(const prime_iterator &lhs, const prime_iterator &rhs) throw() { return lhs.ptr!=rhs.ptr; } //!< !=
            inline friend bool operator!=(const prime_iterator &lhs, const natural &rhs)        throw() { return neq(lhs,rhs); }     //!< !=
            inline friend bool operator!=(const natural &lhs, const prime_iterator &rhs)        throw() { return neq(rhs,lhs); }     //!< !=
            inline friend bool operator!=(const prime_iterator &lhs, const utype rhs)           throw() { return neq(lhs,rhs); }     //!< !=
            inline friend bool operator!=(const utype lhs, const prime_iterator &rhs)           throw() { return neq(rhs,lhs); }     //!< !=

            //! forwarding comparison
#define Y_APP_ITER_CMP(OP) \
inline friend bool operator OP (const prime_iterator &lhs, const prime_iterator &rhs) throw() { return *lhs OP *rhs; }\
inline friend bool operator OP (const prime_iterator &lhs, const natural        &rhs) throw() { return *lhs OP  rhs; }\
inline friend bool operator OP (const natural        &lhs, const prime_iterator &rhs) throw() { return lhs  OP *rhs; }\
inline friend bool operator OP (const prime_iterator &lhs, const utype           rhs) throw() { return *lhs OP  rhs; }\
inline friend bool operator OP (const utype           lhs, const prime_iterator &rhs) throw() { return lhs  OP *rhs; }\

            //! implement all comparisons
#define Y_APP_ITER_COMPARISONS() \
Y_APP_ITER_CMP(<)                \
Y_APP_ITER_CMP(>)                \
Y_APP_ITER_CMP(<=)               \
Y_APP_ITER_CMP(>=)

            Y_APP_ITER_COMPARISONS()

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

