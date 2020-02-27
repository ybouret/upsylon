//! \file

#ifndef Y_INFO_ANY2BASE64_INCLUDED
#define Y_INFO_ANY2BASE64_INCLUDED 1

#include "y/information/translator/base64.hpp"

namespace upsylon {

    namespace Information {

        template <typename T>
        class Any2Base64
        {
        public:
            static const size_t TypeSize = sizeof(T);
            static const size_t NameSize = Y_BASE64_RAW_BYTES_FOR(TypeSize);
            static const size_t NameBytes = NameSize + 1;
            
            virtual ~Any2Base64() throw()
            {
                memset(content,0,sizeof(content));
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Any2Base64);
            char content[ Y_ROUND4(NameBytes) ];
        };
    }
}

#endif

