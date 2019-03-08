
#include "y/lang/pattern/common.hpp"


namespace upsylon
{
    namespace Lang
    {

        const char Common::INTEGER[]         = "[0-9]+";

        const char Common:: C_IDENTIFIER[]   = "[_a-zA-Z][_a-zA-Z0-9]*";

#define Y_POSITIVE_DOUBLE_TEXT "[0-9]+([.][0-9]*([eE][-+]?[:digit:]+)?)?"
        const char Common::POSITIVE_DOUBLE[] = Y_POSITIVE_DOUBLE_TEXT;

        const char Common::DOUBLE[] = "[-+]?" Y_POSITIVE_DOUBLE_TEXT;

    }
}
