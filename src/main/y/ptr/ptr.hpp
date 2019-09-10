//! \file
#ifndef Y_PTR_INCLUDED
#define Y_PTR_INCLUDED 1

#include "y/type/args.hpp"
#include <iostream>

namespace upsylon
{

    namespace core
    {
        //! base class for common data
        class ptr
        {
        public:
            virtual ~ptr() throw();  //!< cleanup
            static const char nil[]; //!< "(nil)"
        protected:                   //|
            explicit ptr() throw();  //!< setup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ptr);
        };

    }

    //! base class for smart pointers
    template <typename T>
    class ptr : public core::ptr
    {
    public:
        Y_DECL_ARGS(T,type);                //!< type traits helper
        typedef mutable_type *pointee_type; //!< for internal handling

        //! just clean up
        inline virtual ~ptr() throw() { pointee=0; }

        //! transitive operator
        inline type       * operator->() throw()       { assert(pointee); return  pointee; }
        //! transitive operator, const
        inline const_type * operator->() const throw() { assert(pointee); return  pointee; }
        //! access operator
        inline type       & operator*() throw()        { assert(pointee); return *pointee; }
        //! access operator, const
        inline const_type & operator*() const throw()  { assert(pointee); return *pointee; }

        //! check validity
        inline bool is_valid() const throw() { return 0!=pointee; }

        //! check validity
        inline bool is_empty() const throw() { return 0==pointee; }

        //! output forwarding
        inline friend std::ostream & operator<<( std::ostream &os, const ptr &p )
        {
            if(p.pointee)
            {
                os << *(p.pointee);
            }
            else
            {
                os << nil;
            }
            return os;
        }

        //! testing equality
        inline friend bool operator==( const ptr<T> &lhs, const ptr<T> &rhs) throw()
        {
            return (lhs.pointee==rhs.pointee);
        }

        //! testing difference
        inline friend bool operator!=( const ptr<T> &lhs, const ptr<T> &rhs) throw()
        {
            return (lhs.pointee!=rhs.pointee);
        }

        //! for comparison forwarding
        inline friend bool operator<(const ptr<T> &lhs, const ptr<T> &rhs) throw()
        {
            assert(lhs.pointee);
            assert(rhs.pointee);
            return (*lhs) < (*rhs);
        }

        
    protected:
        pointee_type pointee; //!< internal pointer

        //! transfert address
        inline explicit ptr(T *addr) throw() : core::ptr(), pointee( (pointee_type)addr ) {}
        //! copy address
        inline ptr(const ptr &other) throw() : core::ptr(), pointee( other.pointee ) {}

        //! swap ownership
        inline void swap_with( ptr<T> &other ) throw()
        {
            cswap(pointee,other.pointee);
        }

        //! set pointee to NULL, no matter what
        inline void zero() const throw() { *(pointee_type *)( &(this->pointee) ) = 0;  }

    private:
        Y_DISABLE_ASSIGN(ptr);
    };
    
}

#endif

