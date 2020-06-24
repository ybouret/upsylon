//! \file
#ifndef Y_TYPE_BINARY_INCLUDED
#define Y_TYPE_BINARY_INCLUDED 1

#include "y/type/args.hpp"
#include <iosfwd>

namespace upsylon {

    //--------------------------------------------------------------------------
    //
    //! base class for binary copy of (persistent) types
    //
    //--------------------------------------------------------------------------
    class binary_
    {
    public:
        const size_t   size; //!< internal size
        const uint8_t *byte; //!< internal bytes

        virtual ~binary_() throw(); //!< cleanup

        friend std::ostream & operator<<(std::ostream  &, const binary_ &);         //!< display
        friend bool           operator==(const binary_ &, const binary_ &) throw(); //!< test equality
        friend bool           operator!=(const binary_ &, const binary_ &) throw(); //!< test difference


    protected:
        explicit binary_( const size_t n, const uint8_t *p) throw(); //!< setup
        void     fill_with( const void *p ) throw();                 //!< fill with data

    private:
        Y_DISABLE_COPY_AND_ASSIGN(binary_);


    };

    //--------------------------------------------------------------------------
    //
    //! derived classes for binary content
    //
    //--------------------------------------------------------------------------
    template <typename T>
    class binary : public binary_
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //! setup
        inline binary( param_type args ) throw() :
        binary_( sizeof(data), data ), data()
        {
            fill_with( &args );
        }

        //! cleanup
        inline virtual ~binary() throw() {}

        //! copy
        inline binary( const binary &other ) throw() :
        binary_( sizeof(data), data ), data()
        {
            fill_with( other.data );
        }

        //! assign
        inline binary & operator=( const binary &other ) throw()
        {
            if( this != &other )
            {
                fill_with(other.data);
            }
            return *this;
        }

        //! assign other data
        inline binary & operator=( param_type args ) throw()
        {
            fill_with( &args );
            return *this;
        }

    private:
        const uint8_t data[sizeof(T)];
     };

}

#endif

