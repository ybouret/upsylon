#include "y/math/fit/gaussians.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            void Gaussians:: Create(Variables &var, const size_t n)
            {
                var.free();
                for(unsigned i=1;i<=unsigned(n);++i)
                {
                    const string a   = vformat("a%u",i);
                    const string mu  = vformat("mu%u",i);
                    const string sig = vformat("sig%u",i);
                    var << a << mu << sig;
                }
                assert( var.size() == 3*n );
            }
        }
        
    }
}
