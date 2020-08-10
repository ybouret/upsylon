//! \file
#ifndef Y_PTR_EMBEDDED_INCLUDED
#define Y_PTR_EMBEDDED_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! embedding a derived pointer managed by a base class PTR
    //
    //__________________________________________________________________________
    template <typename T, typename U, template <class> class PTR >
    class embedded
    {
    public:
        Y_DECL_ARGS(T,type);         //!< aliases
        typedef PTR<U> pointer_type; //!< smart pointer on base blass

        //! take a user created derived pointer
        inline  embedded( const_type *p ) throw() : handle_( (mutable_type *)p ), pointer( handle_ ) {}

        //! use default constructor
        inline  embedded() : handle_( new mutable_type() ), pointer( handle_ ) {}
        
        //! copy using pointer semantics
        inline embedded( const embedded &other ) throw() : handle_( other.handle_ ), pointer( other.pointer ) {}
        
        //! cleanup
        inline virtual ~embedded() throw() { handle_=0; }

        inline type *       operator->() throw()       { assert(handle_); return handle_; }   //!< pointer on derived
        inline const_type * operator->() const throw() { assert(handle_); return handle_; }   //!< pointer on derived
        inline type &       operator*()  throw()       { assert(handle_); return *handle_; }  //!< derived access
        inline const_type & operator*()  const throw() { assert(handle_); return *handle_; }  //!< derived access


    private: mutable_type       *handle_; //!< retain the base class
    public:  const pointer_type  pointer; //!< pointer type with specific semantic
    private:
        Y_DISABLE_ASSIGN(embedded);
    };
}


#endif

