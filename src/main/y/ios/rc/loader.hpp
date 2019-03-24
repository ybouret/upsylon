//! \file
#ifndef Y_IOS_RC_LOADER_INLCUDED
#define Y_IOS_RC_LOADER_INCLUDED 1

#include "y/ios/irstream.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon
{
    namespace ios
    {

        //! load resources appended to the end of a file
        class rc
        {
        public:
            static const uint32_t MAGIC = Y_FOURCC('Y', '_' , 'R', 'C');

            explicit rc( const string &filename );
            virtual ~rc() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rc);
        };

    }
}

#endif

