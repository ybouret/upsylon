
//! \file

#ifndef Y_IOS_SCRIBE_1D_INCLUDED
#define Y_IOS_SCRIBE_1D_INCLUDED 1

#include "y/ios/scribe.hpp"

namespace upsylon
{
    namespace ios
    {
        class scribe1D : public scribe
        {
        public:
            virtual ~scribe1D() throw();

        protected:
            explicit scribe1D(const std::type_info &tid,
                              const size_t          tsz);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(scribe1D);
        };
    }

}

#endif

