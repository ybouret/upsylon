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
    class binary_format_
    {
    public:
        const size_t   size; //!< internal size
        const uint8_t *byte; //!< internal bytes

        virtual ~binary_format_() throw(); //!< cleanup

        friend std::ostream & operator<<(std::ostream  &, const binary_format_ &);         //!< display
        friend bool           operator==(const binary_format_ &, const binary_format_ &) throw(); //!< test equality
        friend bool           operator!=(const binary_format_ &, const binary_format_ &) throw(); //!< test difference


    protected:
        explicit binary_format_(const size_t n, const uint8_t *p) throw(); //!< setup
        void     fill_with(const void *p) throw();                 //!< fill with data

    private:
        Y_DISABLE_COPY_AND_ASSIGN(binary_format_);


    };


    //--------------------------------------------------------------------------
    //
    //! derived classes for binary content
    //
    //--------------------------------------------------------------------------
    template <typename T>
    class binary_format : public binary_format_
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //! setup
        inline binary_format( param_type args ) throw() :
        binary_format_( sizeof(data), data ), data()
        {
            fill_with( &args );
        }

        //! cleanup
        inline virtual ~binary_format() throw() {}

        //! copy
        inline binary_format( const binary_format &other ) throw() :
        binary_format_( sizeof(data), data ), data()
        {
            fill_with( other.data );
        }

        //! assign
        inline binary_format & operator=( const binary_format &other ) throw()
        {
            if( this != &other )
            {
                fill_with(other.data);
            }
            return *this;
        }

        //! assign other data
        inline binary_format & operator=( param_type args ) throw()
        {
            fill_with( &args );
            return *this;
        }

    private:
        const uint8_t data[sizeof(T)];
    };

    //! to be used as iomanip
    template <typename T> inline
    binary_format<T> binary(const T &args) throw()
    {
        return binary_format<T>(args);
    }
}

#endif

