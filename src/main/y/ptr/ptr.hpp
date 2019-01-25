//! \file
#ifndef Y_PTR_INCLUDED
#define Y_PTR_INCLUDED 1

#include "y/type/args.hpp"
#include <iostream>

namespace upsylon
{

    //! base class for smart pointers
    template <typename T>
    class ptr
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
                os << "(nil)";
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
        
    protected:
        pointee_type pointee; //!< internal pointer

        //! transfert address
        inline explicit ptr(T *addr) throw() : pointee( (pointee_type)addr ) {}
        //! copy address
        inline ptr(const ptr &other) throw() : pointee( other.pointee ) {}

        //! swap ownership
        inline void swap_with( ptr<T> &other ) throw()
        {
            cswap(pointee,other.pointee);
        }
    private:
        Y_DISABLE_ASSIGN(ptr);
    };
    
}

#endif

