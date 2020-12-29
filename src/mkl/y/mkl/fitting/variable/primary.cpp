
#include "y/mkl/fitting/variable/primary.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            primary_variable:: ~primary_variable() throw()
            {
            }

            size_t primary_variable:: check_index(const size_t the_index) const
            {
                if(the_index<=0) throw exception("invalid fitting primary variable index=0");
                return the_index;
            }

            size_t primary_variable:: get_index() const throw()
            {
                return indx;
            }
        }
    }
}
