
#include "y/mkl/fitting/variable.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            variable:: ~variable() throw()
            {
            }

            const string & variable:: key() const throw()
            {
                return name;
            }

            std::ostream & operator<<(std::ostream &os, const variable &v)
            {
                os << '\'' << v.name << '\'' << '@' << *v;
                return os;
            }

            
        }

    }

}

