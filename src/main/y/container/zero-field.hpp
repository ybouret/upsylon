//! \file
#ifndef Y_ZERO_FIELD_INCLUDED
#define Y_ZERO_FIELD_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{

        //! memory for a zero field
        class zero_field_
        {
        public:
            virtual ~zero_field_() throw();           //!< cleanup

            const size_t   item_size;                 //!< item size
        protected:                                    //|
            void           *item_addr;                //!< item memory
            explicit zero_field_(const size_t bytes); //!< initialize

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zero_field_);
        };

        //! implementation of a zero field
        template <typename T>
        class zero_field : public zero_field_
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            const_type &zero;    //!< the 'zero' object for the implementation

            //! C++ setup
            inline explicit zero_field() :
            zero_field_(sizeof(type)),
            zero( *( new (item_addr) type() ) )
            {

            }

            //! C++ cleanup
            inline virtual ~zero_field() throw()
            {
                destruct( static_cast<mutable_type *>(item_addr) );
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(zero_field);
        };

}

#endif

