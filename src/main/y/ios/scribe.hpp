//! \file

#ifndef Y_IOS_SCRIBE_INCLUDED
#define Y_IOS_SCRIBE_INCLUDED 1

#include "y/string.hpp"
#include <typeinfo>

namespace upsylon
{
    namespace ios
    {

        //! base class to convert a type into a string
        class scribe : public object, public counted
        {
        public:
            virtual ~scribe() throw();

            
        protected:
            explicit scribe(const std::type_info &tid) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribe);
        };

    }
}

#endif
