#include "y/math/fit/global-variable.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            GlobalVariable:: GlobalVariable(const string & __name, const size_t __indx ) :
            Variable(__name,IsGlobal),
            indx(__indx)
            {
                if(indx<=0) throw exception("%s.indx<=0", *name );
            }

            GlobalVariable:: ~GlobalVariable() throw()
            {
            }

            size_t GlobalVariable:: index() const throw() { return indx; }

        }

    }
}
