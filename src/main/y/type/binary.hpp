//! \file
#ifndef Y_TYPE_BINARY_INCLUDED
#define Y_TYPE_BINARY_INCLUDED 1

#include "y/type/args.hpp"
#include <iosfwd>

namespace upsylon {

    class binary_
    {
    public:
        const size_t   size;
        const uint8_t *byte;

        virtual ~binary_() throw();

        friend std::ostream & operator<<( std::ostream &, const binary_ &);
        friend bool           operator==(const binary_ &, const binary_ &) throw();
        friend bool           operator!=(const binary_ &, const binary_ &) throw();


    protected:
        explicit binary_( const size_t n, const uint8_t *p) throw();
        void     fill_with( const void *p ) throw();

    private:
        Y_DISABLE_COPY_AND_ASSIGN(binary_);


    };

    template <typename T>
    class binary : public binary_
    {
    public:
        Y_DECL_ARGS(T,type);

        inline binary( param_type args ) throw() :
        binary_( sizeof(data), data ), data()
        {
            fill_with( &args );
        }

        inline virtual ~binary() throw() {}

        inline binary( const binary &other ) throw() :
        binary_( sizeof(data), data ), data()
        {
            fill_with( other.data );
        }

        inline binary & operator=( const binary &other ) throw()
        {
            if( this != &other )
            {
                fill_with(other.data);
            }
            return *this;
        }

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

