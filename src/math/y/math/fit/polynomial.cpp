#include "y/math/fit/polynomial.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            void Polynomial:: Create(Variables &vars, const size_t n)
            {
                vars.free();
                for(unsigned i=0;i<unsigned(n);++i)
                {
                    const string a = vformat("a%u",i);
                    vars << a;
                }
            }
        }
    }
}
