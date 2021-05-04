//! \file

#ifndef Y_IOS_SCRIBE_INCLUDED
#define Y_IOS_SCRIBE_INCLUDED 1

#include "y/string.hpp"
#include "y/type/rtti.hpp"

namespace upsylon
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class to convert a type into a string
        //
        //______________________________________________________________________
        class scribe : public object, public counted
        {
        public:
            virtual       ~scribe() throw();               //!< cleanup
            virtual string write(const void *) const = 0;  //!< virtual interface

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            const   rtti  &key() const throw(); //!< for int_ptr

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const rtti  &info; //!< unique type info
            const size_t size; //!< sizeof(object)
            const size_t dims; //!< number of components: 0=size%dims

        protected:
            explicit scribe(const std::type_info &tid,
                            const size_t          tsz,
                            const size_t          num); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribe);
        };

        
    }
}

#endif
