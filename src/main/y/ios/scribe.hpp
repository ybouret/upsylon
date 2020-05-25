//! \file

#ifndef Y_IOS_SCRIBE_INCLUDED
#define Y_IOS_SCRIBE_INCLUDED 1

#include "y/ios/ostream.hpp"
#include <typeinfo>

namespace upsylon {

    namespace ios {

        class scribe : public object
        {
        public:
            virtual ~scribe() throw();

            const std::type_info &info;

        protected:
            explicit scribe(const std::type_info &) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribe);
        };

    }
}

#endif

