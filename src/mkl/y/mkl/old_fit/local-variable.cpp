#include "y/math/fit/local-variable.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            LocalVariable:: LocalVariable( const string & __name, const Variable::Pointer &__link ) :
            Variable( __name, IsLocal ),
            link(__link)
            {
            }

            LocalVariable:: ~LocalVariable() throw() {}

            size_t LocalVariable:: index() const throw() { return link->index(); }
        }
    }
}
