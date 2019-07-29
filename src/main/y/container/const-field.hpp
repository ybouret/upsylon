//! \file
#ifndef Y_CONST_FIELD_INCLUDED
#define Y_CONST_FIELD_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{

        //! memory for a const field
        class const_field_
        {
        public:
            virtual ~const_field_() throw();           //!< cleanup
            const size_t   item_size;                  //!< item size
        protected:                                     //|
            void           *item_addr;                 //!< item memory
            explicit const_field_(const size_t bytes); //!< initialize

        private:
            Y_DISABLE_COPY_AND_ASSIGN(const_field_);
        };

        //! implementation of a zero field
        template <typename T>
        class const_field : public const_field_
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            const_type &value;   //!< the 'value' object for the implementation

            //! C++ setup, default constructor on zeroed bytes
            inline explicit const_field() :
            const_field_(sizeof(type)),
            value( *( new (item_addr) type() ) )
            {

            }

            //! C++ setup with one argument constructor
            template <typename U>
            inline explicit const_field( const U &args ) :
            const_field_(sizeof(type)),
            value( *( new (item_addr) type(args) ) )
            {

            }

            //! C++ setup with two arguments constructor
            template <typename U, typename V>
            inline explicit const_field( const U &argU, const V &argV ) :
            const_field_(sizeof(type)),
            value( *( new (item_addr) type(argU,argV) ) )
            {

            }

            //! C++ cleanup
            inline virtual ~const_field() throw()
            {
                destruct( static_cast<mutable_type *>(item_addr) );
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(const_field);
        };

}

#endif

