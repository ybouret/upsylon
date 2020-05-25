//! \file

#ifndef Y_IOS_SCRIBE_BASIC_INCLUDED
#define Y_IOS_SCRIBE_BASIC_INCLUDED 1

#include "y/ios/scribe.hpp"
#include "y/string.hpp"

namespace upsylon {

    namespace ios {

        class basic_scribe : public scribe
        {
        public:
            virtual ~basic_scribe() throw();
            const string fmt;

        protected:
            template <typename FMT> inline
            explicit basic_scribe(const std::type_info &t, const FMT &f) :
            scribe(t,1), fmt(f)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(basic_scribe);
        };


    }
}

#endif

