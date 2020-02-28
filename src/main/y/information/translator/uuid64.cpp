#include "y/information/translator/uuid64.hpp"
#include "y/information/translator/base64.hpp"

namespace upsylon {

    namespace core {

        using namespace Information;

        void uuid64::encode(char *target, const size_t targetLength, const void *source, const size_t sourceLength) throw()
        {
            static const Base64::Mode mode = Base64::Raw;
            assert(target);
            assert(targetLength>0);
            assert(source);
            assert(sourceLength>0);
            assert( targetLength ==  Base64::BytesFor(sourceLength,mode) );
            Base64::Encode(target,source,sourceLength,Base64::std_table,mode);
            target[ targetLength ] = 0;
        }
    }

}

