//! \file

#ifndef Y_IOS_SCRIBE_INCLUDED
#define Y_IOS_SCRIBE_INCLUDED 1

#include "y/string.hpp"
#include "y/type/rtti.hpp"

namespace upsylon
{
    namespace ios
    {

        //! base class to convert a type into a string
        class scribe : public object, public counted
        {
        public:
            virtual       ~scribe() throw();
            virtual string write(const void *) const = 0;
      
            const rtti &info;

        protected:
            explicit scribe(const std::type_info &tid);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribe);
        };

    }
}

#endif
