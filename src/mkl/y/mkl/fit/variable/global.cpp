
#include "y/mkl/fit/variable/global.hpp"

namespace upsylon {

    namespace math {

        namespace Fit {

            GlobalVariable:: ~GlobalVariable() throw()
            {

            }


            GlobalVariable:: GlobalVariable( const string &n, const size_t i) :
            Variable(n),
            position(i)
            {
            }

            GlobalVariable:: GlobalVariable( const char   *n, const size_t i) :
            Variable(n),
            position(i)
            {
            }

            GlobalVariable:: GlobalVariable(const GlobalVariable &other) :
            Variable(other),
            position(other.position)
            {
            }

            size_t GlobalVariable:: index() const throw()
            {
                return position;
            }



        }
    }
}


