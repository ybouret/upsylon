//! \file
#ifndef Y_SPARSE_ZERO_FIELD_INCLUDED
#define Y_SPARSE_ZERO_FIELD_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{

    namespace sparse
    {
        //! memory for a zero field
        class zero_field_
        {
        public:
            virtual ~zero_field_() throw();               //!< cleanup

        protected:
            explicit zero_field_(const size_t item_size); //!< initialize
            size_t   itsz_;                               //!< item size
            void    *item_;                               //!< item memory

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zero_field_);
        };

        //! implementation of a zero field
        template <typename T>
        class zero_field : public zero_field_
        {
        public:
            Y_DECL_ARGS(T,type);
            const_type &zero;

            //! C++ setup
            inline explicit zero_field() :
            zero_field_(sizeof(type)),
            zero( *( new (item_) type() ) )
            {

            }

            //! C++ cleanup
            inline virtual ~zero_field() throw()
            {
                destruct( static_cast<mutable_type *>(item_) );
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(zero_field);
        };

    }
}

#endif

