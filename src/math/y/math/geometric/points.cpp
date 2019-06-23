#include "y/math/geometric/points.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon  {

    namespace math
    {

        namespace Geometric
        {

            static inline
            void throw_null_norm(const char *ctx)
            {
                if(!ctx) ctx = "???";
                throw libc::exception(EDOM,"null norm(%s)", ctx);
            }

            template <>
            float CheckNorm2<float>(const float value, const char *ctx)
            {
                if(value<=0) throw_null_norm(ctx);
                return value;
            }

            template <>
            double CheckNorm2<double>(const double value, const char *ctx)
            {
                if(value<=0) throw_null_norm(ctx);
                return value;
            }
        }

    }
}
